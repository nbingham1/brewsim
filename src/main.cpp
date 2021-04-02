#include <iostream>

#include "order.h"
#include "simulator.h"
#include "load.h"

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

