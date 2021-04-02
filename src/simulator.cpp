#include "simulator.h"
#include <math.h>

Status::Status() {
}

Status::Status(const Process &process) {
	std::set<int32_t> exprs;
	for (size_t i = 0; i < process.expressions.size(); i++) {
		exprs.insert(i);
	}

	evaluate(process, exprs);

	for (auto i = process.start.begin(); i != process.start.end(); i++) {
		this->curr.insert(std::pair<int32_t, int64_t>(i->first, getValue(i->second)));
	}
	
	evaluate(process, exprs);
}

Status::Status(const Status &copy) {
	this->schedule = copy.schedule;
	this->curr = copy.curr;
	this->values = copy.values;
}

Status::~Status() {
}

int64_t Status::getValue(Term term) const {
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

void Status::print(const Process &process, Term term) const {
	if (term.type == Term::RESOURCE) {
		auto i = curr.find(term.value);
		if (i == curr.end()) {
			printf("%s:null", process.resources[term.value].name.c_str());
		} else {
			printf("%s:%ld", process.resources[term.value].name.c_str(), i->second);
		}
	} else if (term.type == Term::EXPRESSION) {
		const Expression &expr = process.expressions[term.value];
		if (expr.terms.size() == 1) {
			printf("(");
			if (expr.operators.size() > 0) {
				if (expr.operators[0] == Expression::NOT) {
					printf("!");
				} else if (expr.operators[0] == Expression::NEG) {
					printf("-");
				}
			}

			print(process, expr.terms[0]);
			printf("):%ld", values[term.value]);
		} else {
			printf("(");
			for (size_t i = 0; i < expr.terms.size(); i++) {
				if (i > 0) {
					printf("%s", Expression::opStr(expr.operators[i-1]).c_str());
				}
				print(process, expr.terms[i]);
			}
			printf("):%ld\n", values[term.value]);
		}
	} else {
		printf("%ld", term.value);
	}
}

bool Status::step(const Process &process, int32_t taskId) {
	printf("Step %d\n", taskId);
	std::set<int32_t> exprs;

	auto i = curr.begin();
	auto j = process.tasks[taskId].requirements.begin();
	while (i != curr.end() and j != process.tasks[taskId].requirements.end()) {
		if (i->first == j->first) {
			int64_t amount = getValue(j->second.amount);
			if (j->second.type != Utilization::PRODUCE && i->second < amount) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), i->second, amount);
				return false;
			}

			if (j->second.type == Utilization::CONSUME) {
				i->second -= amount;
			} else if (j->second.type == Utilization::PRODUCE) {
				i->second += amount;
			}

			if (j->second.type == Utilization::CONSUME or j->second.type == Utilization::PRODUCE) {
				exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			}
			i++;
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			int64_t amount = getValue(j->second.amount);
			if (j->second.type == Utilization::PRODUCE) {
				curr.insert(i, std::pair<int32_t, int64_t>(j->first, amount));
				
				exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			} else if (amount > 0) {
				printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), 0l, amount);
				return false;
			}

			j++;
		}
	}

	while (j != process.tasks[taskId].requirements.end()) {
		int64_t amount = getValue(j->second.amount);
		if (j->second.type == Utilization::PRODUCE) {
			curr.insert(i, std::pair<int32_t, int64_t>(j->first, amount));

			exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
		} else if (amount > 0) {
			printf("not enough %s: %ld < %ld\n", process.resources[j->first].name.c_str(), 0l, amount);
			return false;
		}

		j++;
	}

	for (auto i = curr.begin(); i != curr.end(); i++) {
		printf("%s: %ld\n", process.resources[i->first].name.c_str(), i->second);
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

		int64_t value = 0;
		print(process, expr.terms[0]);
		if (expr.operators.size() == 1 and expr.operators[0] >= Expression::ABS) {
			int64_t operands[2] = {0, 0};

			value = getValue(Term(Term::EXPRESSION, *exprId));
			printf("%s(", Expression::opStr(expr.operators[0]).c_str());
			for (size_t i = 0; i < expr.terms.size() and i < 2; i++) {
				if (i != 0) {
					printf(", ");
				}
				operands[i] = getValue(expr.terms[i]);
				print(process, expr.terms[i]);
			}
			printf(")");
			switch (expr.operators[0]) {
				case Expression::ABS: value = operands[0] < 0 ? -operands[0] : operands[0]; break;
				case Expression::SUM: value += operands[0]; break;
				case Expression::PSUM: value += operands[0]; break; // TODO(nbingham)
				case Expression::MIN: value = operands[0] < operands[1] ? operands[0] : operands[1]; break;
				case Expression::MAX: value = operands[0] > operands[1] ? operands[0] : operands[1]; break;
				case Expression::LOG: value = (int64_t)(log(operands[1])/log(operands[0])); break;
				case Expression::POW: value = (int64_t)pow(operands[0], operands[1]); break;
			}
		} else if (expr.terms.size() == 1) {
			value = getValue(expr.terms[0]);
			switch (expr.operators[0]) {
				case Expression::NOT: value = (int64_t)(not value); break;
				case Expression::NEG: value = -value; break;
			}
		} else {
			value = getValue(expr.terms[0]);
			for (size_t i = 1; i < expr.terms.size(); i++) {
				int64_t next = getValue(expr.terms[i]);
				printf("%s", Expression::opStr(expr.operators[i-1]).c_str());
				print(process, expr.terms[i]);
				switch (expr.operators[i-1]) {
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
		printf("\n");

		if (*exprId >= (int32_t)values.size()) {
			values.resize(*exprId+1, 0);
		}
		values[*exprId] = value;

		exprs.insert(expr.parents.begin(), expr.parents.end());
	}
}

bool Status::satisfies(const std::map<int32_t, Term> &task) const {
	auto i = curr.begin();
	auto j = task.begin();
	while (i != curr.end() and j != task.end()) {
		if (i->first == j->first) {
			if (i->second < getValue(j->second)) {
				return false;
			}
			
			i++;
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			if (getValue(j->second) > 0) {
				return false;
			}

			j++;
		}
	}

	while (j != task.end()) {
		if (getValue(j->second) > 0) {
			return false;
		}
		j++;
	}

	return true;
}

bool Status::satisfies(const Process &process, const std::vector<Term> &constraints) const {
	for (auto i = constraints.begin(); i != constraints.end(); i++) {
		printf("constraint %ld: ", i-constraints.begin());
		print(process, *i);
		printf("\n");
		if (getValue(*i) == 0) {
			return false;
		}
	}
	return true;
}

Simulator::Simulator() {
}

Simulator::~Simulator() {
}

void Simulator::run(const Process &process) {
	this->stack.clear();
	this->stack.push_back(Status(process));

	minima.resize(process.minimize.size());
	maxima.resize(process.maximize.size());

	while (stack.size() > 0) {
		printf("stack size: %lu\n", stack.size());
		Status status = std::move(stack.back());
		stack.pop_back();

		if (status.satisfies(process.end)) {
			printf("satisfies end condition\n");
			for (size_t i = 0; i < process.minimize.size(); i++) {
				if (minima[i].schedule.size() == 0 or status.getValue(process.minimize[i]) < minima[i].getValue(process.minimize[i])) {
					minima[i] = status;
				}
			}
			
			for (size_t i = 0; i < process.maximize.size(); i++) {
				if (maxima[i].schedule.size() == 0 or status.getValue(process.maximize[i]) > maxima[i].getValue(process.maximize[i])) {
					maxima[i] = status;
				}
			}
		} else {
			for (size_t taskId = 0; taskId < process.tasks.size(); taskId++) {
				printf("checking task %lu\n", taskId);
				Status choice = status;
				choice.schedule.push_back(taskId);
				if (choice.step(process, taskId) and choice.satisfies(process, process.constraints)) {
					printf("success\n");
					// TODO(nbingham) check if seen
					bool found = false;
					for (size_t i = 0; not found and i < process.minimize.size(); i++) {
						found = found or minima[i].schedule.size() == 0 or status.getValue(process.minimize[i]) < minima[i].getValue(process.minimize[i]);
					}
					
					for (size_t i = 0; not found and i < process.maximize.size(); i++) {
						found = found or maxima[i].schedule.size() == 0 or status.getValue(process.maximize[i]) > maxima[i].getValue(process.maximize[i]);
					}

					if (found) {
						stack.push_back(choice);
					}
				} else {
					printf("fail\n");
				}
			}
		}
	}
}
