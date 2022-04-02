#ifndef DZVALUE_H
#define DZVALUE_H

#include <llvm/IR/Value.h>

#include "Stack.h"
#include "Utility.h"
#include "EntryPoint.h"

namespace antlr4
{
	class ParserRuleContext;
}

struct DzResult
{
	EntryPoint entryPoint;
	Stack values;
};

class DzValue
{
	public:
		DzValue();

		size_t id() const;

		virtual int order(const EntryPoint &entryPoint) const;

		virtual std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const = 0;

	private:
		size_t m_id;
};

#endif // DZVALUE_H
