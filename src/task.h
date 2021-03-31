#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <string>

struct Utilization
{
	enum Type {
		use,
		lock,
		consume,
		produce
	};

	int64_t amount;
	Type type;
};

struct Task
{
	std::string name;
	// resourceId -> utilization
	std::map<int32_t, Utilization> requirements;
};

struct Process
{
	std::vector<Task> tasks;

	// resourceName
	std::vector<std::string> resources;
};
