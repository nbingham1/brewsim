#include "task.h"

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

int32_t Process::getResourceId(std::string resource) {
	for (size_t i = 0; i < resources.size(); i++) {
		if (resources[i].name == resource) {
			return (int32_t)i;
		}
	}
	resources.push_back(Resource(resource));
	return (int32_t)resources.size()-1;
}
