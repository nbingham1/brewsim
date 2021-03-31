#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <string>

struct Utilization
{
	enum {
		USE = 0,
		LOCK = 1,
		CONSUME = 2,
		PRODUCE = 3
	};

	int64_t amount;
	int8_t type;
};

struct Task
{
	std::string name;
	// resourceId ->
	std::map<int32_t, Utilization> requirements;
};

struct Resource
{
	Resource(std::string name);
	~Resource();

	std::string name;
	std::set<int32_t> parents;
};

struct Expression
{
	enum {
		NOT = 0,
		NEG = 1,
		AND = 2,
		OR = 3,
		LT = 4,
		GT = 5,
		LE = 6,
		GE = 7,
		EQ = 8,
		NE = 9,
		ADD = 10,
		SUB = 11,
		MUL = 12,
		DIV = 13,
		MOD = 14
	};

	std::string label;
	int32_t op;
	std::set<int32_t> parents;
	std::vector<int32_t> children;
};

struct Process
{
	// resourceName
	std::vector<Resource> resources;
	std::vector<Task> tasks;

	std::vector<Expression> expressions;
	std::vector<int32_t> constraints;
	std::vector<int32_t> minimize;
	std::vector<int32_t> maximize;

	// resourceId -> amount
	std::map<int32_t, int64_t> start;
	std::map<int32_t, int64_t> end;

	int32_t getResourceId(std::string resource);
};
