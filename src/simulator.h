#pragma once

#include "task.h"
#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

struct Status;

struct Step
{
	Step();
	Step(const Status &status, size_t taskId);
	~Step();

	size_t taskId;
	// resourceId -> amount
	std::map<int32_t, int64_t> have;
	std::vector<int64_t> values;
	int32_t branches;

	Step *prev;

	int64_t getValue(Term term) const;
	bool mergeAndCheck(const Process &process, std::map<int32_t, int64_t> *need);
};

struct Status
{
	Status();
	Status(const Process &process);
	Status(const Status &copy);
	~Status();

	Step *prev;

	// resourceId -> amount
	std::map<int32_t, int64_t> have;

	std::vector<int64_t> values;

	void drop();
	int64_t getValue(Term term) const;
	void print(const Process &process, Term term) const;
	void print(const Process &process) const;

	int64_t psum(const Process &process, Term term, int64_t next) const;

	bool step(const Process &process, int32_t taskId);
	void evaluate(const Process &process, std::set<int32_t> exprs);
	bool satisfies(const Process &process, const std::vector<Term> &constraints) const;

};

struct Simulator
{
	Simulator();
	~Simulator();

	std::vector<Status> stack;
	std::vector<Status> minima;
	std::vector<Status> maxima;

	void reset();
	void run(const Process &process);
};

