#ifndef NAMEDVALUE_H
#define NAMEDVALUE_H

#include <llvm/IR/BasicBlock.h>

#include "BaseValue.h"
#include "DzValue.h"

class DzTypeName;

class NamedValue : public BaseValue
{
	public:
		NamedValue(const std::string &name
			, const BaseValue *value
			, const Type *type
			);

		std::string name() const;

		const BaseValue *value() const;

		const Type *type() const override;

		bool hasValue() const;

	private:
		std::string m_name;

		const BaseValue *m_value;
		const Type *m_type;
};

#endif // NAMEDVALUE_H
