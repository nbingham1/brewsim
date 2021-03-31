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
	// resourceId ->
	std::map<int32_t, Utilization> requirements;
};

struct Process
{
	// resourceName
	std::vector<std::string> resources;
	std::vector<Task> tasks;

	// resourceId -> amount
	std::map<int32_t, int64_t> start;
	std::map<int32_t, int64_t> end;

	int32_t getResourceId(std::string resource);
};
