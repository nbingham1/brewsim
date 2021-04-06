#include <iostream>

#include "order.h"
#include "simulator.h"
#include "load.h"

int main(int argc, char **argv)
{
	printf("%d %d %d\n", '\r', '\n', '\t');

	if (argc > 1) {
		order_t order;
		grammar_t grammar;
		order.load(grammar);

		lexer_t lexer;
		lexer.open(argv[1]);

		parsing result = grammar.parse(lexer);
		std::string tmp = lexer.read(result.tree.end-2, result.tree.end+1);
		printf("%d %d %d\n", tmp[0], tmp[1], tmp[2]);
		if (result.msgs.size() == 0) {
			Process proc;
			load(&proc, lexer, order, result.tree);
			Simulator sim;
			if (sim.run(proc)) {
				printf("\n");

				for (auto i = sim.minima.begin(); i != sim.minima.end(); i++) {
					i->print(proc);
					printf("\n");
				}

				for (auto i = sim.maxima.begin(); i != sim.maxima.end(); i++) {
					i->print(proc);
					printf("\n");
				}
			} else {
				sim.error.print(proc);
				std::cout << "error: " << sim.error.msg << std::endl;
			}
		} else {
			for (auto msg : result.msgs) {
				std::cout << msg << std::endl;
			}
		}
	}
}

