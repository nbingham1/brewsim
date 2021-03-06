#pragma once

#include <pgen/grammar.h>

namespace pgen
{

struct order_t
{
	int32_t ORDER;
	int32_t _;
	int32_t HAVE;
	int32_t NEED;
	int32_t VARIABLE;
	int32_t SELECT;
	int32_t CONSTRAINT;
	int32_t RULE;
	int32_t EXPRESSION;
	int32_t INSTANCE;
	int32_t ASSIGNS;
	int32_t TEXT;
	int32_t ASSIGN;
	int32_t EXPRESSION0;
	int32_t EXPRESSION1;
	int32_t EXPRESSION2;
	int32_t EXPRESSION3;
	int32_t EXPRESSION4;
	int32_t EXPRESSION5;
	int32_t INTEGER;
	int32_t FUNCTION;

	void load(grammar_t &grammar);
};

}

