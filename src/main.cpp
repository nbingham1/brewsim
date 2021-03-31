#include <iostream>

#include "order.h"
#include "simulator.h"

using namespace pgen;

bool loadResource(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	auto i = token.tokens.begin();
	std::string type = lexer.read(i->begin, i->end);
	i++;

	while (i != token.tokens.end()) {
		// TODO(nbingham) implement expressions
		std::string expr = lexer.read(i->begin, i->end);
		int64_t amount = 0;
		if (expr == "inf") {
			amount = std::numeric_limits<int64_t>::max();
		} else {
			amount = std::stoi(expr);
		}
		i++;

		std::string name = lexer.read(i->begin, i->end);
		int32_t index = p->getResourceId(name);
		i++;

		printf("%s %d %ld\n", type.c_str(), index, amount);

		if (type == "have") {
			p->start.insert(std::pair<int32_t, int64_t>(index, amount));
		} else if (type == "need") {
			p->end.insert(std::pair<int32_t, int64_t>(index, amount));
		}
	}

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
		// TODO(nbingham) implement expressions
		std::string expr = lexer.read(i->begin, i->end);
		result.amount = std::stoi(expr);
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

	printf("found task %s\n", result.name.c_str());
	p->tasks.push_back(result);
	for (auto r : result.requirements) {
		printf("%d %ld\n", r.first, r.second.amount);
	}

	return true;
}

bool load(Process *p, lexer_t &lexer, const order_t &gram, const token_t &token) {
	for (auto i : token.tokens) {
		if (i.type == gram.TASK) {
			if (not loadTask(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.RESOURCE) {
			if (not loadResource(p, lexer, gram, i)) {
				return false;
			}
		} else if (i.type == gram.VARIABLE) {
		} else if (i.type == gram.SELECT) {
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
			Simulator sim;
			sim.run(proc);
		} else {
			for (auto msg : result.msgs) {
				std::cout << msg << std::endl;
			}
		}
	}
}

