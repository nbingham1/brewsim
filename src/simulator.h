#pragma once

#include "task.h"
#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

struct Status;
struct Step;

struct State
{
	State();
	State(const State &state);
	~State();

	std::map<ResourceID, int64_t> have;
	std::vector<int64_t> values;

	int64_t getValue(Term term) const;
	bool step(const Process &process, const Step *prev, int32_t taskId);
	void evaluate(const Process &process, const Step *prev, std::set<int32_t> exprs);

	int64_t sum(const Step *prev, Term last, Term term) const;
	int64_t makespan(const Process &process, const Step *prev, Term last, Term term) const;

	void print(const Process &process, Term term) const;
	void print(const Process &process) const;
};

struct Step
{
	Step();
	Step(const Status &status, size_t taskId);
	~Step();

	size_t taskId;
	State state;
	int32_t branches;

	Step *prev;

	void print(const Process &process) const;
};

struct Status
{
	Status();
	Status(const Process &process);
	Status(const Status &copy);
	~Status();

	Step *prev;
	State state;

	std::string msg;
	int64_t length;

	void drop();
	void print(const Process &process) const;
};

struct Simulator
{
	Simulator();
	~Simulator();

	std::vector<Status> stack;
	std::vector<Status> minima;
	std::vector<Status> maxima;
	Status error;

	bool optimizes(const Status &status, const Process &process) const;
	void reset();
	bool run(const Process &process);
};

