#pragma once

#include "task.h"

struct Status
{
	Status(std::vector<int32_t> start);
	Status(const Status &copy);
	~Status();

	// indexes into arcs
	std::vector<int32_t> curr;
};

struct Simulator
{
	Simulator(OrderGraph *orders);
	~Simulator();

	OrderGraph *orders;

	std::vector<Status> stack;

	void setup();
	void run();
};

