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

int64_t Status::getValue(Term term) {
	if (term.type == Term::RESOURCE) {
		auto i = curr.find(term.value);
		if (i == curr.end()) {
			return 0;
		} else {
			return i->second;
		}
	} else if (term.type == Term::EXPRESSION) {
		return values[term.value];
	} else {
		return term.value;
	}
}

bool Status::step(const Process &process, int32_t taskId) {
	std::set<int32_t> exprs;

	auto i = curr.begin();
	auto j = process.tasks[taskId].requirements.begin();
	while (i != curr.end() and j != process.tasks[taskId].requirements.end()) {
		if (i->first == j->first) {
			if (j->second.type != Utilization::PRODUCE && i->second < j->second.amount) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), i->second, j->second.amount);
				return false;
			}

			if (j->second.type == Utilization::CONSUME) {
				i->second -= j->second.amount;
			} else if (j->second.type == Utilization::PRODUCE) {
				i->second += j->second.amount;
			}

			if (j->second.type == Utilization::CONSUME or j->second.type == Utilization::PRODUCE) {
				exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			}
			i++;
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			if (j->second.type == Utilization::PRODUCE) {
				curr.insert(i, std::pair<int32_t, int64_t>(j->first, j->second.amount));
				
				exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			} else if (j->second.amount > 0) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), 0l, j->second.amount);
				return false;
			}

			j++;
		}
	}

	while (j != process.tasks[taskId].requirements.end()) {
		if (j->second.type == Utilization::PRODUCE) {
			curr.insert(i, std::pair<int32_t, int64_t>(j->first, j->second.amount));

			exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
		} else if (j->second.amount > 0) {
			printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), 0l, j->second.amount);
			return false;
		}

		j++;
	}

	evaluate(process, exprs);

	return true;
}

void Status::evaluate(const Process &process, std::set<int32_t> exprs)
{
	// The parser/interpreter must guarantee that expressions with higher Id
	// values are always dependant upon only expressions with lower Id values.
	for (auto exprId = exprs.begin(); exprId != exprs.end(); exprId++) {
		const Expression &expr = process.expressions[*exprId];

		int64_t value = getValue(expr.terms[0]);
		if (expr.op == Expression::NOT) {
			value = (int64_t)(not value);
		} else if (expr.op == Expression::NEG) {
			value = -value;
		} else {
			for (auto i = expr.terms.begin()+1; i != expr.terms.end(); i++) {
				int64_t next = getValue(*i);
				switch (expr.op) {
					case Expression::AND: value = (int64_t)(value and next); break;
					case Expression::OR: value = (int64_t)(value or next); break;
					case Expression::LT: value = (int64_t)(value < next); break;
					case Expression::GT: value = (int64_t)(value > next); break;
					case Expression::LE: value = (int64_t)(value <= next); break;
					case Expression::GE: value = (int64_t)(value >= next); break;
					case Expression::EQ: value = (int64_t)(value == next); break;
					case Expression::NE: value = (int64_t)(value != next); break;
					case Expression::ADD: value = (int64_t)(value + next); break;
					case Expression::SUB: value = (int64_t)(value - next); break;
					case Expression::MUL: value = (int64_t)(value * next); break;
					case Expression::DIV: value = (int64_t)(value / next); break;
					case Expression::MOD: value = (int64_t)(value % next); break;
				}
			}
		}

		values[*exprId] = value;

		exprs.insert(expr.parents.begin(), expr.parents.end());
	}
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
