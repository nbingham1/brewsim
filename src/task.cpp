#include "task.h"

std::string Expression::opStr(int32_t op) {
	switch (op) {
		case Expression::AND: return " and ";
		case Expression::OR: return " or ";
		case Expression::LT: return " < ";
		case Expression::GT: return " > ";
		case Expression::LE: return " <= ";
		case Expression::GE: return " >= ";
		case Expression::EQ: return " == ";
		case Expression::NE: return " != ";
		case Expression::ADD: return " + ";
		case Expression::SUB: return " - ";
		case Expression::MUL: return " * ";
		case Expression::DIV: return " / ";
		case Expression::MOD: return " %% ";
	}
	return " . ";
}

Resource::Resource(std::string name) {
	this->name = name;
}

Resource::~Resource() {
}

Term::Term() {
	this->type = CONSTANT;
	this->value = 0;
}

Term::Term(int8_t type, int64_t value) {
	this->type = type;
	this->value = value;
}

Term::~Term() {
}

Task::Task() {
	guard = Term(Term::CONSTANT, 1);
}

Task::~Task() {
}

Process::Process() {
	constraints = Term(Term::CONSTANT, 1);
}

Process::~Process() {
}

int32_t Process::getResourceId(std::string resource) {
	for (size_t i = 0; i < resources.size(); i++) {
		if (resources[i].name == resource) {
			return (int32_t)i;
		}
	}
	resources.push_back(Resource(resource));
	return (int32_t)resources.size()-1;
}
