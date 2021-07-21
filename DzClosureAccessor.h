#ifndef DZCLOSUREACCESSOR_H
#define DZCLOSUREACCESSOR_H

#include "DzValue.h"
#include "EntryPoint.h"

class DzClosureAccessor : public DzValue
{
	public:
		DzClosureAccessor(DzValue *subject)
			: m_subject(subject)
		{
		}

	public:
		llvm::Value *build(const EntryPoint &entryPoint) const override
		{
			auto locals = entryPoint.locals();
			auto closure = locals["parent"]->build(entryPoint);

			auto nested = entryPoint
				.withClosure(closure);

			return m_subject->build(nested);
		}

	private:
		DzValue *m_subject;
};

#endif // DZCLOSUREACCESSOR_H
