#ifndef USERTYPEVALUE_H
#define USERTYPEVALUE_H

#include <vector>

#include "BaseValue.h"

#include "metadata/UserTypeValueMetadata.h"

class NamedValue;
class IPrototype;

class UserTypeValue : public BaseValueWithMetadata<UserTypeValueMetadata>
{
	public:
		UserTypeValue(const IPrototype *type, const std::vector<const NamedValue *> &values);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

		const IPrototype *prototype() const;

		std::vector<const NamedValue *> fields() const;

	private:
		const IPrototype *m_type;

		std::vector<const NamedValue *> m_values;
};

#endif // USERTYPEVALUE_H
