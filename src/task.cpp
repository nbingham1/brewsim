#include "task.h"

int32_t Process::getResourceId(std::string resource) {
	auto pos = std::find(resources.begin(), resources.end(), resource);
	int32_t index = -1;
	if (pos == resources.end()) {
		index = (int32_t)resources.size();
		resources.push_back(resource);
	} else {
		index = (int32_t)(pos - resources.begin());
	}
	return index;
}
