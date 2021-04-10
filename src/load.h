#pragma once

#include "order.h"
#include "simulator.h"

using namespace pgen;

int32_t loadOperator(lexer_t &lexer, const token_t &token);

Term loadFunction(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

Term loadExpression(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadHave(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadNeed(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadAssign(Process *p, Task *t, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadAssigns(Process *p, Task *t, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadTask(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadVariable(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadConstraint(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool loadSelect(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

bool load(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token);

