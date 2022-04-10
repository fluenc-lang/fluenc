#ifndef NAMEDVALUE_H
#define NAMEDVALUE_H

#include <llvm/IR/BasicBlock.h>

#include "BaseValue.h"
#include "Node.h"

class ITypeName;

class NamedValue : public BaseValue
{
	public:
		NamedValue(const std::string &name, const BaseValue *value);

		std::string name() const;

		const BaseValue *value() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

	private:
		std::string m_name;

		const BaseValue *m_value;
};

#endif // NAMEDVALUE_H
