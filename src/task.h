#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <string>

struct Term
{
	enum {
		CONSTANT = 0,
		RESOURCE = 1,
		EXPRESSION = 2
	};

	Term();
	Term(int8_t type, int64_t value);
	~Term();

	int8_t type;
	int64_t value;
};

struct Utilization
{
	enum {
		USE = 0,
		LOCK = 1,
		CONSUME = 2,
		PRODUCE = 3
	};

	Term amount;
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

	std::vector<int32_t> operators;
	std::set<int32_t> parents;
	std::vector<Term> terms;

	static std::string opStr(int32_t);
};

struct Process
{
	// resourceName
	std::vector<Resource> resources;
	std::vector<Task> tasks;

	std::vector<Expression> expressions;
	std::map<std::string, Term> variables;
	std::vector<Term> constraints;
	std::vector<Term> minimize;
	std::vector<Term> maximize;

	// resourceId -> amount
	std::map<int32_t, Term> start;
	std::map<int32_t, Term> end;

	int32_t getResourceId(std::string resource);
};
