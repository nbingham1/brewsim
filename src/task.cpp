#include "task.h"

Resource::Resource(std::string name) {
	this->name = name;
}

Resource::~Resource() {
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
