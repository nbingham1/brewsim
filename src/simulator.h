#pragma once

#include "task.h"
#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

struct Status
{
	Status();
	Status(std::map<int32_t, int64_t> start);
	Status(const Status &copy);
	~Status();

	// taskId
	std::vector<size_t> schedule;

	// resourceId -> amount
	std::map<int32_t, int64_t> curr;

	bool step(const Process &process, int32_t taskId);
	bool satisfies(const std::map<int32_t, int64_t> &task) const;
};

struct Simulator
{
	Simulator();
	~Simulator();

	std::vector<Status> stack;
	std::unordered_set<std::set<int32_t>, std::vector<int64_t> > seen;
	Status minima;

	void run(const Process &process);
};

