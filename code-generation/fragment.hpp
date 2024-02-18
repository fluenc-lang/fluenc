#pragma once

#include <vector>

class EntryPoint;
class Emitter;

struct DefaultVisitorContext;
struct DzResult;

struct fragment
{
	virtual std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const = 0;
};
