#pragma once

#include <pgen/grammar.h>

namespace pgen
{

struct order_t
{
	int32_t ORDER;
	int32_t _;
	int32_t RESOURCE;
	int32_t VARIABLE;
	int32_t AGGREGATE;
	int32_t CONSTRAINT;
	int32_t TASK;
	int32_t EXPRESSION;
	int32_t INSTANCE;
	int32_t BOOLEAN;
	int32_t TEXT;
	int32_t EFFECT;
	int32_t BOOLEAN0;
	int32_t BOOLEAN1;
	int32_t EXPRESSION0;
	int32_t EXPRESSION1;
	int32_t INTEGER;

	void load(grammar_t &grammar);
};

}

