#ifndef FORWARDEDVALUE_H
#define FORWARDEDVALUE_H

#include "BaseValue.h"

class ForwardedValue : public BaseValue
{
	public:
		ForwardedValue(const BaseValue *subject);

		const BaseValue *subject() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;
		const ValueMetadata &metadata() const override;

	private:
		const BaseValue *m_subject;
};

#endif // FORWARDEDVALUE_H
