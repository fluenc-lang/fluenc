#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"
#include "IteratorTypeHandle.h"

class EntryPoint;
class DzTypeName;

class LazyValue : public BaseValue
{
	public:
		LazyValue(const IteratorTypeHandle &handle, const DzValue *subject, const EntryPoint &entryPoint);

		std::vector<DzResult> build(llvm::BasicBlock *block, const Stack &values) const;

		const Type *type() const override;

	private:
		const IteratorTypeHandle m_handle;

		const DzValue *m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // LAZYVALUE_H
