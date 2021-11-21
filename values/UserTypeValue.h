#ifndef USERTYPEVALUE_H
#define USERTYPEVALUE_H

#include "BaseValue.h"

class NamedValue;

class UserTypeValue : public BaseValue
{
	public:
		UserTypeValue(const Type *type, const std::vector<const NamedValue *> &values);

		const Type *type() const override;

		std::vector<const NamedValue *> fields() const;

	private:
		const Type *m_type;

		std::vector<const NamedValue *> m_values;
};

#endif // USERTYPEVALUE_H
