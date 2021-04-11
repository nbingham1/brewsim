#include "simulator.h"
#include <math.h>
#include <iostream>

State::State() {
}

State::State(const State &state) {
	this->have = state.have;
	this->values = state.values;
}

State::~State() {
}

int64_t State::getValue(Term term) const {
	if (term.type == Term::RESOURCE) {
		auto i = have.find(term.value);
		if (i == have.end()) {
			return 0;
		} else {
			return i->second;
		}
	} else if (term.type == Term::EXPRESSION) {
		if (term.value < (int64_t)values.size()) {
			return values[term.value];
		} else {
			return 0;
		}
	} else {
		return term.value;
	}
}

bool State::step(const Process &process, const Step *prev, int32_t taskId) {
	if (getValue(process.tasks[taskId].guard) == 0) {
		return false;
	}

	std::set<int32_t> exprs;

	auto i = have.begin();
	auto j = process.tasks[taskId].actions.back().begin();
	while (i != have.end() and j != process.tasks[taskId].actions.back().end()) {
		if (i->first == j->first) {
			int64_t amount = getValue(j->second);

			exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			if (amount == 0) {
				i = have.erase(i);
			} else {
				i->second = amount;
				i++;
			}
			j++;
		} else if (i->first < j->first) {
			i++;
		} else {
			int64_t amount = getValue(j->second);

			exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
			if (amount != 0) {
				have.insert(i, std::pair<int32_t, int64_t>(j->first, amount));
			}
			j++;
		}
	}

	while (j != process.tasks[taskId].actions.back().end()) {
		int64_t amount = getValue(j->second);

		exprs.insert(process.resources[j->first].parents.begin(), process.resources[j->first].parents.end());
		if (amount != 0) {
			have.insert(i, std::pair<int32_t, int64_t>(j->first, amount));
		}
		j++;
	}

	/*for (auto i = have.begin(); i != have.end(); i++) {
		printf("%s: %lld\n", process.resources[i->first].name.c_str(), i->second);
	}*/

	evaluate(process, prev, exprs);

	return getValue(process.constraints) != 0;
}

void State::evaluate(const Process &process, const Step *prev, std::set<int32_t> exprs)
{
	// The parser/interpreter must guarantee that expressions with higher Id
	// values are always dependant upon only expressions with lower Id values.
	for (auto exprId = exprs.begin(); exprId != exprs.end(); exprId++) {
		const Expression &expr = process.expressions[*exprId];

		int64_t value = 0;
		//print(process, expr.terms[0]);
		if (expr.operators.size() == 1 and expr.operators[0] >= Expression::ABS) {
			int64_t operands[2] = {0, 0};

			//printf("%s(", Expression::opStr(expr.operators[0]).c_str());
			for (size_t i = 0; i < expr.terms.size() and i < 2; i++) {
				//if (i != 0) {
					//printf(", ");
				//}
				operands[i] = getValue(expr.terms[i]);
				//print(process, expr.terms[i]);
			}
			//printf(")");
			switch (expr.operators[0]) {
				case Expression::ABS: value = operands[0] < 0 ? -operands[0] : operands[0]; break;
				case Expression::MIN: value = operands[0] < operands[1] ? operands[0] : operands[1]; break;
				case Expression::MAX: value = operands[0] > operands[1] ? operands[0] : operands[1]; break;
				case Expression::LOG: value = (int64_t)(log(operands[1])/log(operands[0])); break;
				case Expression::POW: value = (int64_t)pow(operands[0], operands[1]); break;
				case Expression::DIFF:
					value = operands[0];
					if (prev != nullptr) {
						value -= prev->state.getValue(expr.terms[0]);
					}
					break;
				case Expression::SUM:
					value = operands[0];
					if (prev != nullptr) {
						value += prev->state.getValue(Term(Term::EXPRESSION, *exprId));
					}
					break;
				case Expression::MAKESPAN:
					value = makespan(process, prev, Term(Term::EXPRESSION, *exprId), expr.terms[0]);
					break;
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
				//printf("%s", Expression::opStr(expr.operators[i-1]).c_str());
				//print(process, expr.terms[i]);
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
		//printf("\n");

		if (*exprId >= (int32_t)values.size()) {
			values.resize(*exprId+1, 0);
		}
		values[*exprId] = value;

		exprs.insert(expr.parents.begin(), expr.parents.end());
	}
}

int64_t State::sum(const Step *prev, Term last, Term term) const {
	if (prev != nullptr) {
		return prev->state.getValue(last) + getValue(term);
	}
	return 0;
}

int64_t State::makespan(const Process &process, const Step *prev, Term last, Term term) const {
	std::map<int32_t, int64_t> need;

	const Step *curr = prev;
	const Step *start = nullptr;
	while (curr != nullptr) {
		start = curr;
		curr = curr->prev;

		if (curr->state.getValue(process.tasks[prev->taskId].guard) == 0) {
			break;
		}

		if (prev->state.getValue(process.tasks[curr->taskId].guard) == 0) {
			break;
		}

		if (prev->state.getValue(process.tasks[curr->taskId].guard) == 0) {
			break;
		}
	}
	
	int64_t value0 = 0;
	int64_t value1 = 0;
	if (prev != nullptr) {
		value0 = prev->state.getValue(last);
		value1 = prev->state.getValue(term) - getValue(term);
	}
	if (start != nullptr) {
		value1 += start->state.getValue(last);
	}
	return value1 > value0 ? value1 : value0;
}

void State::print(const Process &process, Term term) const {
	if (term.type == Term::RESOURCE) {
		auto i = have.find(term.value);
		if (i == have.end()) {
			printf("%s:null", process.resources[term.value].name.c_str());
		} else {
			printf("%s:%lld", process.resources[term.value].name.c_str(), i->second);
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
			printf("):%lld", values[term.value]);
		} else {
			printf("(");
			for (size_t i = 0; i < expr.terms.size(); i++) {
				if (i > 0) {
					printf("%s", Expression::opStr(expr.operators[i-1]).c_str());
				}
				print(process, expr.terms[i]);
			}
			printf("):%lld\n", values[term.value]);
		}
	} else {
		printf("%lld", term.value);
	}
}

void State::print(const Process &process) const {
	for (auto i = process.variables.begin(); i != process.variables.end(); i++) {
		if (i != process.variables.begin()) {
			printf(", ");
		}
		printf("%s = %lld", i->first.c_str(), getValue(i->second));
	}
	if (process.variables.size() != 0) {
		printf("\n");
	}
	for (auto i = have.begin(); i != have.end(); i++) {
		if (i != have.begin()) {
			printf(", ");
		}
		printf("%lld %s", i->second, process.resources[i->first].name.c_str());
	}
	if (have.size() != 0) {
		printf("\n");
	}
}

Step::Step() {
	this->prev = nullptr;
	this->branches = 1;
}

Step::Step(const Status &status, size_t taskId) {
	this->taskId = taskId;
	this->state = status.state;
	this->prev = status.prev;
	if (this->prev != nullptr) {
		this->prev->branches += 1;
	}
	this->branches = 1;
}

Step::~Step() {
}

/*bool Step::mergeAndCheck(const Process &process, std::map<int32_t, int64_t> *need) {
	auto i = need->begin();
	auto j = process.tasks[this->taskId].actions.back().begin();
	auto k = have.begin();
	const std::map<int32_t, Term> &action = process.tasks[this->taskId].actions.back();

	while (i != need->end() and j != action.end() and k != have.end()) {
		if (k->first == i->first and i->first == j->first) {
			i->second += getValue(j->second);
			if (k->second < i->second) {
				return false;
			}
			k++;
			i++;
			j++;
		} else if (k->first < i->first and k->first < j->first) {
			k++;
		} else if (k->first > i->first or k->first > j->first) {
			return false;
		} else if (i->first < j->first) {
			if (k->second < i->second) {
				return false;
			}
			k++;
			i++;
		} else {
			int64_t value = getValue(j->second);
			if (k->second < value) {
				return false;
			}
			need->insert(i, std::pair<int32_t, int64_t>(j->first, value));
			k++;
			j++;
		}
	}

	while (k != have.end() and i != need->end()) {
		if (k->first < i->first) {
			k++;
		} else if (k->first > i->first) {
			return false;
		} else {
			if (k->second < i->second) {
				return false;
			}
			k++;
			i++;
		}
	}

	while (k != have.end() and j != action.end()) {
		if (k->first < j->first) {
			k++;
		} else if (k->first > j->first) {
			return false;
		} else {
			int64_t value = getValue(j->second);
			if (k->second < value) {
				return false;
			}
			need->insert(need->end(), std::pair<int32_t, int64_t>(j->first, value));
			k++;
			j++;
		}
	}

	return i == need->end() and j == action.end();
}*/

void Step::print(const Process &process) const {
	printf("%s\n", process.tasks[taskId].name.c_str());
	state.print(process);
}

Status::Status() {
	this->prev = nullptr;
	this->length = 0;
}

Status::Status(const Process &process) {
	this->prev = nullptr;
	this->length = 0;

	std::set<int32_t> exprs;
	for (size_t i = 0; i < process.expressions.size(); i++) {
		exprs.insert(i);
	}

	state.evaluate(process, nullptr, exprs);

	for (auto i = process.start.begin(); i != process.start.end(); i++) {
		this->state.have.insert(std::pair<int32_t, int64_t>(i->first, state.getValue(i->second)));
	}
	
	state.evaluate(process, nullptr, exprs);
}

Status::Status(const Status &copy) {
	this->state = copy.state;
	this->prev = copy.prev;
}

Status::~Status() {
}

void Status::drop() {
	while (prev and (--prev->branches) == 0) {
		Step *curr = prev->prev;
		delete prev;
		prev = curr;
	}
	prev = nullptr;
}

void Status::print(const Process &process) const
{
	if (process.minimize.size() != 0) {
		printf("min: ");
		for (auto i = process.minimize.begin(); i != process.minimize.end(); i++) {
			if (i != process.minimize.begin()) {
				printf(", ");
			}
			printf("%lld", state.getValue(*i));
		}
		printf("\n");
	}

	if (process.maximize.size() != 0) {
		printf("max: ");
		for (auto i = process.maximize.begin(); i != process.maximize.end(); i++) {
			if (i != process.maximize.begin()) {
				printf(", ");
			}
			printf("%lld", state.getValue(*i));
		}
		printf("\n");
	}

	state.print(process);

	Step *curr = prev;
	while (curr != nullptr) {
		curr->print(process);
		printf("\n");
		curr = curr->prev;
	}
}

Simulator::Simulator() {
}

Simulator::~Simulator() {
	reset();
}

bool Simulator::optimizes(const Status &status, const Process &process) const {
	bool found = minima.size() == 0 and maxima.size() == 0;
	for (size_t i = 0; not found and i < process.minimize.size(); i++) {
		found = found or minima[i].prev == nullptr or status.state.getValue(process.minimize[i]) < minima[i].state.getValue(process.minimize[i]);
	}
	
	for (size_t i = 0; not found and i < process.maximize.size(); i++) {
		found = found or maxima[i].prev == nullptr or status.state.getValue(process.maximize[i]) > maxima[i].state.getValue(process.maximize[i]);
	}

	return found;
}

void Simulator::reset() {
	for (auto i = stack.begin(); i != stack.end(); i++) {
		i->drop();
	}
	stack.clear();

	for (auto i = minima.begin(); i != minima.end(); i++) {
		i->drop();
	}
	minima.clear();

	for (auto i = maxima.begin(); i != maxima.end(); i++) {
		i->drop();
	}
	maxima.clear();
}

bool Simulator::run(const Process &process) {
	bool success = false;

	this->stack.clear();
	this->stack.push_back(Status(process));

	minima.resize(process.minimize.size());
	maxima.resize(process.maximize.size());

	int64_t count = 0;
	while (stack.size() > 0) {
		printf("\rstack %llu step %lld", stack.size(), count);
		fflush(stdout);
		Status status = std::move(stack.back());
		stack.pop_back();

		if (status.state.getValue(process.end) != 0) {
			if (process.minimize.size() == 0 and process.maximize.size() == 0) {
				minima.push_back(status);
				printf("\r                    \r");
				return true;
			} else {
				bool found = false;
				for (size_t i = 0; i < process.minimize.size(); i++) {
					if (minima[i].prev == nullptr or status.state.getValue(process.minimize[i]) < minima[i].state.getValue(process.minimize[i])) {
						minima[i] = status;
						found = true;
					}
				}
				
				for (size_t i = 0; i < process.maximize.size(); i++) {
					if (maxima[i].prev == nullptr or status.state.getValue(process.maximize[i]) > maxima[i].state.getValue(process.maximize[i])) {
						maxima[i] = status;
						found = true;
					}
				}

				if (not found) {
					status.drop();
				} else {
					success = true;
				}
			}
		} else {
			for (size_t taskId = 0; taskId < process.tasks.size(); taskId++) {
				Status choice = status;
				choice.prev = new Step(choice, taskId);
				choice.length += 1;
				//printf("Task: %s\n", process.tasks[taskId].name.c_str());
				//choice.print(process, process.tasks[taskId].guard);
				//printf("\n");
				if (choice.state.step(process, choice.prev, taskId)
					and optimizes(choice, process)) {
					// TODO(nbingham) check if seen
					stack.push_back(choice);
				} else if (choice.length > error.length) {
					/*error.print(process);
					std::cout << "error: " << error.msg << std::endl;
					printf("\n\n");*/
					error.drop();
					error = choice;
				} else {
					choice.drop();
				}
			}
		}
	}

	printf("\r                    \r");

	return success;
}

