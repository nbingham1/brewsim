#include <iostream>

#include "order.h"
#include "simulator.h"

int main(int argc, char **argv)
{
	if (argc > 1) {
		pgen::order_t order;
		pgen::grammar_t grammar;
		order.load(grammar);

		pgen::lexer_t lexer;
		lexer.open(argv[1]);

		pgen::parsing result = grammar.parse(lexer);
		if (result.msgs.size() == 0) {
			result.tree.emit(lexer);
		} else {
			for (auto msg : result.msgs) {
				std::cout << msg << std::endl;
			}
		}
	}
}

