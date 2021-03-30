#include "simulator.h"

Status::Status(std::vector<int32_t> start) {
	this->curr = start;
}

Status::Status(const Status &copy) {
	this->curr = copy.curr;
}

Status::~Status() {
}

Simulator::Simulator(OrderGraph *orders) {
	this->orders = orders;
}

Simulator::~Simulator() {
}

void Simulator::setup() {
	stack.push_back(Status(orders->start));
}

void Simulator::run() {
	while (stack.size() > 0) {
		Status status = stack.back();
		stack.pop_back();

		int32_t count = 0;
		for (auto i : status.curr) {
			stack.push_back(status);
			//stack.back().
		}

		if (count == 0) {
			// the full order schedule has been evaluated
			
		}
	}
}
