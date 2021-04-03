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
	} else if (op == "psum") {
		return Expression::PSUM;
	} else if (op == "min") {
		return Expression::MIN;
	} else if (op == "max") {
		return Expression::MAX;
	} else if (op == "log") {
		return Expression::LOG;
	} else if (op == "pow") {
		return Expression::POW;
	} else {
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
		result.terms.push_back(loadExpression(p, lexer, gram, *i));
		i++;
	}
	
	p->expressions.push_back(result);
	return Term(Term::EXPRESSION, p->expressions.size()-1);
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
		if (term.type == Term::EXPRESSION) {
			p->expressions[term.value].parents.insert(p->expressions.size());
		} else if (term.type == Term::RESOURCE) {
			p->resources[term.value].parents.insert(p->expressions.size());
		}
		p->expressions.push_back(result);
		return Term(Term::EXPRESSION, p->expressions.size()-1);
	} else {
		Expression result;
		Term term = loadExpression(p, lexer, gram, *i);
		result.terms.push_back(term);
		if (term.type == Term::EXPRESSION) {
			p->expressions[term.value].parents.insert(p->expressions.size());
		} else if (term.type == Term::RESOURCE) {
			p->resources[term.value].parents.insert(p->expressions.size());
		}
		i++;
		
		while (i != token.tokens.end()) {
			result.operators.push_back(loadOperator(lexer, *i));
			i++;

			term = loadExpression(p, lexer, gram, *i);
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

	p->done = loadExpression(p, lexer, gram, *i);

	return true;	
}


bool loadEffect(Process *p, Task *t, lexer_t &lexer, const order_t &gram, const token_t &token) {
	Utilization result;
	auto i = token.tokens.begin();
	std::string type = lexer.read(i->begin, i->end);
	if (type == "uses") {
		result.type = Utilization::USE;
	} else if (type == "locks") {
		result.type = Utilization::LOCK;
	} else if (type == "consumes") {
		result.type = Utilization::CONSUME;
	} else if (type == "produces") {
		result.type = Utilization::PRODUCE;
	} else {
		// TODO(nbingham) flag an error
	}
	i++;

	while (i != token.tokens.end()) {
		result.amount = loadExpression(p, lexer, gram, *i);
		i++;

		std::string name = lexer.read(i->begin, i->end);
		int32_t index = p->getResourceId(name);
		i++;

		t->requirements.insert(std::pair<int32_t, Utilization>(index, result));
	}

	return true;	
}

bool loadTask(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	Task result;
	auto i = token.tokens.begin();
	if (i->type == gram.TEXT) {
		result.name = lexer.read(i->begin, i->end);
		i++;
	} else {
		// TODO(nbingham) flag an error
	}

	while (i != token.tokens.end()) {
		if (i->type == gram.EFFECT) {
			if (not loadEffect(p, &result, lexer, gram, *i)) {
				return false;
			}
		} else {
			// TODO(nbingham) flag an error
		}
		i++;
	}

	//printf("found task %s\n", result.name.c_str());
	p->tasks.push_back(result);
	/*for (auto r : result.requirements) {
		printf("%d %ld\n", r.first, r.second.amount.value);
	}*/

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
	Term expr = loadExpression(p, lexer, gram, *i);
	i++;

	p->constraints.push_back(expr);
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
		if (i.type == gram.TASK) {
			if (not loadTask(p, lexer, gram, i)) {
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

