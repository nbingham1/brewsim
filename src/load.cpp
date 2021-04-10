#include "load.h"

int32_t loadOperator(lexer_t &lexer, const token_t &token) {
	std::string op = lexer.read(token.begin, token.end);
	if (op == "not") {
		return Expression::NOT;
	} else if (op == "-") {
		return Expression::NEG;
	} else if (op == "and") {
		return Expression::AND;
	} else if (op == "or") {
		return Expression::OR;
	} else if (op == "<") {
		return Expression::LT;
	} else if (op == ">") {
		return Expression::GT;
	} else if (op == "<=") {
		return Expression::LE;
	} else if (op == ">=") {
		return Expression::GE;
	} else if (op == "==") {
		return Expression::EQ;
	} else if (op == "!=") {
		return Expression::NE;
	} else if (op == "+") {
		return Expression::ADD;
	} else if (op == "-") {
		return Expression::SUB;
	} else if (op == "*") {
		return Expression::MUL;
	} else if (op == "/") {
		return Expression::DIV;
	} else if (op == "%") {
		return Expression::MOD;
	} else if (op == "abs") {
		return Expression::ABS;
	} else if (op == "sum") {
		return Expression::SUM;
	} else if (op == "makespan") {
		return Expression::MAKESPAN;
	} else if (op == "min") {
		return Expression::MIN;
	} else if (op == "max") {
		return Expression::MAX;
	} else if (op == "log") {
		return Expression::LOG;
	} else if (op == "pow") {
		return Expression::POW;
	} else if (op == "diff") {
		return Expression::DIFF;
	} else {
		printf("unrecognized operator %s\n", op.c_str());
		return -1;
		// TODO(nbingham) flag an error
	}
}

Term loadFunction(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	Expression result;

	auto i = token.tokens.begin();
	result.operators.push_back(loadOperator(lexer, *i));
	i++;

	while (i != token.tokens.end()) {
		Term term = loadExpression(p, lexer, gram, *i);
		result.terms.push_back(term);
		if (term.type == Term::EXPRESSION) {
			p->expressions[term.value].parents.insert(p->expressions.size());
		} else if (term.type == Term::RESOURCE) {
			p->resources[term.value].parents.insert(p->expressions.size());
		}

		i++;
	}
	
	p->expressions.push_back(result);
	return Term(Term::EXPRESSION, p->expressions.size()-1);
}

void setParent(Process *p, Term term) {
	if (term.type == Term::EXPRESSION) {
		p->expressions[term.value].parents.insert(p->expressions.size());
	} else if (term.type == Term::RESOURCE) {
		p->resources[term.value].parents.insert(p->expressions.size());
	}
}

Term loadExpression(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();
	if (token.tokens.size() == 0) {

		// TODO(nbingham) flag an error
		return Term();
	} else if (token.tokens.size() == 1) {
		if (i->type == gram.INSTANCE) {
			std::string name = lexer.read(i->begin, i->end);
			auto j = p->variables.find(name);
			if (j == p->variables.end()) {
				return Term(Term::RESOURCE, p->getResourceId(name));
			} else {
				return j->second;
			}
		} else if (i->type == gram.INTEGER) {
			std::string valueStr = lexer.read(i->begin, i->end);
			int64_t value = 0;
			if (valueStr == "inf") {
				value = std::numeric_limits<int64_t>::max();
			} else {
				value = std::stoi(valueStr);
			}

			return Term(Term::CONSTANT, value);
		} else if (i->type == gram.FUNCTION) {
			return loadFunction(p, lexer, gram, *i);
		}	else {
			return loadExpression(p, lexer, gram, *i);
		}
	} else if (token.type == gram.EXPRESSION3 or token.type == gram.EXPRESSION4) {
		Expression result;

		result.operators.push_back(loadOperator(lexer, *i));
		i++;

		Term term = loadExpression(p, lexer, gram, *i);
		result.terms.push_back(term);
		setParent(p, term);
		p->expressions.push_back(result);
		return Term(Term::EXPRESSION, p->expressions.size()-1);
	} else {
		Expression result;
		Term term = loadExpression(p, lexer, gram, *i);
		result.terms.push_back(term);
		setParent(p, term);
		i++;
		
		while (i != token.tokens.end()) {
			result.operators.push_back(loadOperator(lexer, *i));
			i++;

			term = loadExpression(p, lexer, gram, *i);
			result.terms.push_back(term);
			setParent(p, term);
			i++;
		}

		p->expressions.push_back(result);
		return Term(Term::EXPRESSION, p->expressions.size()-1);
	}
}

bool loadHave(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();

	while (i != token.tokens.end()) {
		Term amount = loadExpression(p, lexer, gram, *i);
		i++;

		std::string name = lexer.read(i->begin, i->end);
		int32_t index = p->getResourceId(name);
		i++;

		//printf("have %d %ld\n", index, amount.value);

		p->start.insert(std::pair<int32_t, Term>(index, amount));
	}

	return true;	
}

bool loadNeed(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();

	Term term = loadExpression(p, lexer, gram, *i);

	if (p->end.type == Term::CONSTANT and p->end.value == 0) {
		p->end = term;
	} else {
		Expression result;
		result.terms.push_back(p->end);
		setParent(p, p->end);
		result.terms.push_back(term);
		setParent(p, term);
		result.operators.push_back(Expression::AND);
		p->expressions.push_back(result);
		p->end = Term(Term::EXPRESSION, p->expressions.size()-1);
	}

	return true;
}

bool loadAssign(Process *p, Task *t, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();
	std::string name = lexer.read(i->begin, i->end);
	int32_t index = p->getResourceId(name);
	i++;

	std::string op = lexer.read(i->begin, i->end);
	i++;

	Term term = loadExpression(p, lexer, gram, *i);
	i++;

	auto u = t->actions.back().find(index);
	if (u == t->actions.back().end()) {
		u = t->actions.back().insert(std::pair<int32_t, Term>(index, Term(0, 0))).first;
	}

	if (op == "=") {
		u->second = term;	
	} else {
		Expression result;
		Term rsc(Term::RESOURCE, index);
		result.terms.push_back(rsc);
		setParent(p, rsc);
		result.terms.push_back(term);
		setParent(p, term);
		if (op == "+=") {
			result.operators.push_back(Expression::ADD);
		} else {
			result.operators.push_back(Expression::SUB);
		}

		p->expressions.push_back(result);
		u->second = Term(Term::EXPRESSION, p->expressions.size()-1);
	}

	return true;	
}

bool loadAssigns(Process *p, Task *t, lexer_t &lexer, const order_t &gram, const token_t &token) {
	t->actions.push_back(std::map<ResourceID, Term>());

	auto i = token.tokens.begin();

	while (i != token.tokens.end()) {
		if (not loadAssign(p, t, lexer, gram, *i)) {
			return false;
		}
		i++;
	}
	
	return true;	
}

bool loadRule(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	Task result;
	auto i = token.tokens.begin();

	result.guard = loadExpression(p, lexer, gram, *i);
	i++;

	loadAssigns(p, &result, lexer, gram, *i);
	i++;

	if (i->type == gram.ASSIGNS) {
		loadAssigns(p, &result, lexer, gram, *i);
		i++;
	}

	result.name = lexer.read(i->begin, i->end);
	i++;

	p->tasks.push_back(result);
	return true;
}

bool loadVariable(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();
	std::string name = lexer.read(i->begin, i->end);
	i++;

	Term expr = loadExpression(p, lexer, gram, *i);
	i++;

	p->variables.insert(std::pair<std::string, Term>(name, expr));
	return true;
}

bool loadConstraint(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();

	Term term = loadExpression(p, lexer, gram, *i);

	if (p->constraints.type == Term::CONSTANT and p->constraints.value == 1) {
		p->constraints = term;
	} else {
		Expression result;
		result.terms.push_back(p->constraints);
		setParent(p, p->constraints);
		result.terms.push_back(term);
		setParent(p, term);
		result.operators.push_back(Expression::AND);
		p->expressions.push_back(result);
		p->constraints = Term(Term::EXPRESSION, p->expressions.size()-1);
	}

	return true;
}

bool loadSelect(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();
	std::string name = lexer.read(i->begin, i->end);
	i++;

	Term expr = loadExpression(p, lexer, gram, *i);
	i++;

	if (name == "min") {
		p->minimize.push_back(expr);
	} else if (name == "max") {
		p->maximize.push_back(expr);
	}
	return true;
}

bool load(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	for (auto i : token.tokens) {
		if (i.type == gram.RULE) {
			if (not loadRule(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.HAVE) {
			if (not loadHave(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.NEED) {
			if (not loadNeed(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.VARIABLE) {
			if (not loadVariable(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.CONSTRAINT) {
			if (not loadConstraint(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.SELECT) {
			if (not loadSelect(p, lexer, gram, i)) {
				return false;
			}
		} else {
			// TODO(nbingham) flag an error
		}
	}

	return true;
}

