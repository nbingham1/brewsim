#include <iostream>

#include "order.h"
#include "simulator.h"

using namespace pgen;

bool loadEffect(Task *t, lexer_t &lexer, const order_t &gram, const token_t &token) {
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
			if (not loadEffect(&result, lexer, gram, *i)) {
				return false;
			}
		} else {
			// TODO(nbingham) flag an error
		}
		i++;
	}

	printf("found task %s\n", result.name.c_str());
	p->tasks.push_back(result);

	return true;
}

bool load(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	for (auto i : token.tokens) {
		if (i.type == gram.TASK) {
			if (not loadTask(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.RESOURCE) {
		} else if (i.type == gram.VARIABLE) {
		} else if (i.type == gram.AGGREGATE) {
		} else if (i.type == gram.CONSTRAINT) {
		} else {
			// TODO(nbingham) flag an error
		}
	}

	return true;
}

int main(int argc, char **argv)
{
	if (argc > 1) {
		order_t order;
		grammar_t grammar;
		order.load(grammar);

		lexer_t lexer;
		lexer.open(argv[1]);

		parsing result = grammar.parse(lexer);
		if (result.msgs.size() == 0) {
			Process proc;
			load(&proc, lexer, order, result.tree);
		} else {
			for (auto msg : result.msgs) {
				std::cout << msg << std::endl;
			}
		}
	}
}

