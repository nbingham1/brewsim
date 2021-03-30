#pragma once

#include <stdint.h>
#include <vector>
#include <string>

struct Arc
{
	int64_t maxSeparation;

	// OR, indexes into tasks
	std::vector<int32_t> from;
	std::vector<int32_t> to;
};

struct Requirement
{
	int32_t resourceId;
	float scale;
};

struct Resource
{
	std::string name;
	float maxPeak;
	float maxAccum;
};

struct Task
{
	int32_t orderId;
	int64_t duration; // minutes
	std::vector<Requirement> resources;

	// AND, indexes into arcs
	std::vector<int32_t> from;
	std::vector<int32_t> to;
};

struct OrderGraph
{
	std::vector<Task> tasks;
	std::vector<Arc> arcs;
	std::vector<Resource> resources;
	
	// indexes into arcs
	std::vector<int32_t> start;
};
