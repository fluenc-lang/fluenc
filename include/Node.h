#ifndef DZVALUE_H
#define DZVALUE_H

#include "Stack.h"
#include "EntryPoint.h"
#include "TokenInfo.h"

struct DzResult
{
	EntryPoint entryPoint;
	Stack values;
};

class Node
{
	public:
		Node();

		size_t id() const;

		virtual int order(const EntryPoint &entryPoint) const;

		virtual std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const = 0;

	private:
		size_t m_id;
};

#endif // DZVALUE_H
