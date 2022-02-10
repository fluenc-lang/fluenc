#ifndef ARRAYVALUE_H
#define ARRAYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class ArrayValue : public BaseValue
{
	public:
		ArrayValue(const EntryPoint &entryPoint
			, const Type *iteratorType
			, const DzValue *subject
			);

		std::vector<DzResult> build(llvm::BasicBlock *block, const Stack &values) const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

	private:
		const EntryPoint *m_entryPoint;
		const Type *m_iteratorType;
		const DzValue *m_subject;
};

#endif // ARRAYVALUE_H
