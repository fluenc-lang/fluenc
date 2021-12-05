#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class EntryPoint;
class DzTypeName;

class LazyValue : public BaseValue
{
	public:
		LazyValue(const DzValue *subject, const EntryPoint &entryPoint);

		std::vector<DzResult> build(llvm::BasicBlock *block, const Stack &values) const;

		const Type *type() const override;

	private:
		const DzValue *m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // LAZYVALUE_H
