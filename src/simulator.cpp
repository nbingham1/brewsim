#include "simulator.h"

Status::Status() {
}

Status::Status(std::map<int32_t, int64_t> start) {
	this->curr = start;
}

Status::Status(const Status &copy) {
	this->curr = copy.curr;
}

Status::~Status() {
}

bool Status::step(const Process &process, int32_t taskId) {
	auto i = curr.begin();
	auto j = process.tasks[taskId].requirements.begin();
	while (i != curr.end() and j != process.tasks[taskId].requirements.end()) {
		if (i->first == j->first) {
			if (j->second.type != Utilization::Type::produce && i->second < j->second.amount) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].c_str(), i->second, j->second.amount);
				return false;
			}

			if (j->second.type == Utilization::Type::consume) {
				i->second -= j->second.amount;
			} else if (j->second.type == Utilization::Type::produce) {
				i->second += j->second.amount;
			}
			i++;
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			if (j->second.type == Utilization::Type::produce) {
				curr.insert(i, std::pair<int32_t, int64_t>(j->first, j->second.amount));
			} else if (j->second.amount > 0) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].c_str(), 0l, j->second.amount);
				return false;
			}
			
			j++;
		}
	}

	while (j != process.tasks[taskId].requirements.end()) {
		if (j->second.type == Utilization::Type::produce) {
			curr.insert(i, std::pair<int32_t, int64_t>(j->first, j->second.amount));
		} else if (j->second.amount > 0) {
			printf("not enough %s: %ld < %ld\n", process.resources[j->first].c_str(), 0l, j->second.amount);
			return false;
		}

		j++;
	}

	return true;
}

bool Status::satisfies(const std::map<int32_t, int64_t> &task) const {
	auto i = curr.begin();
	auto j = task.begin();
	while (i != curr.end() and j != task.end()) {
		if (i->first == j->first) {
			if (i->second < j->second) {
				return false;
			}
			
			i++;
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			if (j->second > 0) {
				return false;
			}

			j++;
		}
	}

	while (j != task.end()) {
		if (j->second > 0) {
			return false;
		}
		j++;
	}

	return true;
}

Simulator::Simulator() {
}

Simulator::~Simulator() {
}

void Simulator::run(const Process &process) {
	this->stack.clear();
	this->stack.push_back(Status(process.start));

	while (stack.size() > 0) {
		printf("stack size: %lu\n", stack.size());
		Status status = std::move(stack.back());
		stack.pop_back();

		if (status.satisfies(process.end)) {
			printf("satisfies end condition\n");
			// TODO(nbingham) check against min
		} else {
			for (size_t taskId = 0; taskId < process.tasks.size(); taskId++) {
				printf("checking task %lu\n", taskId);
				Status choice = status;
				choice.schedule.push_back(taskId);
				if (choice.step(process, taskId)) {
					printf("success\n");
					// TODO(nbingham) check if seen
					// TODO(nbingham) check against min
					stack.push_back(choice);
				} else {
					printf("fail\n");
				}
			}
		}
	}
}
