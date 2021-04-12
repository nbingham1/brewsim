#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <string>

using ResourceID = int32_t;
using ExprID = int32_t;

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

using Action = std::map<ResourceID, Term>;

struct Task
{
	Task();
	~Task();

	std::string name;
	
	Term guard;
	std::vector<Action> actions;
};

struct Resource
{
	Resource(std::string name);
	~Resource();

	std::string name;
	std::set<ExprID> parents;
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
		MOD = 14,
		ABS = 15,
		SUM = 16,
		MAKESPAN = 17,
		MIN = 18,
		MAX = 19,
		LOG = 20,
		POW = 21,
		DIFF = 22
	};

	std::vector<int32_t> operators;
	std::set<ExprID> parents;
	std::vector<Term> terms;

	static std::string opStr(int32_t);
};

struct Process
{
	Process();
	~Process();

	std::vector<Resource> resources;
	std::vector<Task> tasks;

	std::vector<Expression> expressions;
	std::map<std::string, Term> variables;
	std::vector<Term> minimize;
	std::vector<Term> maximize;

	Action start;
	
	Term constraints;
	Term end;

	ResourceID getResourceId(std::string resource);
};
