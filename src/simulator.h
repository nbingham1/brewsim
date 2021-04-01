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
	Status(const Process &process);
	Status(const Status &copy);
	~Status();

	// taskId
	std::vector<size_t> schedule;

	// resourceId -> amount
	std::map<int32_t, int64_t> curr;

	std::vector<int64_t> values;

	int64_t getValue(Term term) const;

	bool step(const Process &process, int32_t taskId);
	void evaluate(const Process &process, std::set<int32_t> exprs);
	bool satisfies(const std::map<int32_t, Term> &task) const;
};

struct Simulator
{
	Simulator();
	~Simulator();

	std::vector<Status> stack;
	std::unordered_set<std::set<int32_t>, std::vector<int64_t> > seen;
	std::vector<Status> minima;
	std::vector<Status> maxima;

	void run(const Process &process);
};

