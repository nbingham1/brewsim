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

bool Status::step(const std::map<int32_t, Utilization> &task) {
	auto i = curr.begin();
	auto j = task.begin();
	while (i != curr.end() and j != task.end()) {
		if (i->first == j->first) {
			if (j->second.type != Utilization::Type::produce && i->second < j->second.amount) {
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
			if (j->second.type != Utilization::Type::produce && j->second.amount > 0) {
				return false;
			}

			curr.insert(i, std::pair<int32_t, int64_t>(j->first, j->second.amount));
			j++;
		}
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
	return true;
}

Simulator::Simulator() {
}

Simulator::~Simulator() {
}

void Simulator::setup(std::map<int32_t, int64_t> start, std::map<int32_t, int64_t> end) {
	this->stack.push_back(Status(start));
	this->end = end;
}

void Simulator::run(const Process &process) {
	while (stack.size() > 0) {
		Status status = std::move(stack.back());
		stack.pop_back();

		if (status.satisfies(end)) {
			// TODO(nbingham) check against min
		} else {
			for (size_t taskId = 0; taskId < process.tasks.size(); taskId++) {
				Status choice = status;
				choice.schedule.push_back(taskId);
				if (choice.step(process.tasks[taskId].requirements)) {
					// TODO(nbingham) check if seen
					// TODO(nbingham) check against min
					stack.push_back(choice);
				}
			}
		}
	}
}
