#ifndef NAMEDVALUE_H
#define NAMEDVALUE_H

#include <llvm/IR/BasicBlock.h>

#include "BaseValue.h"
#include "DzValue.h"

class NamedValue : public BaseValue
{
	public:
		NamedValue(const std::string &name, const EntryPoint &entryPoint, const DzValue *subject);

		std::string name() const;
		std::vector<DzResult> build(llvm::BasicBlock *block, const Stack &values) const;

		const Type *type() const override;

		bool hasValue() const;

	private:
		std::string m_name;

		const EntryPoint *m_entryPoint;
		const DzValue *m_subject;
};

#endif // NAMEDVALUE_H
