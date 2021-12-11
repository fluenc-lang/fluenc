#ifndef TAINTEDVALUE_H
#define TAINTEDVALUE_H

#include "BaseValue.h"

class TaintedValue : public BaseValue
{
	public:
		TaintedValue(const BaseValue *subject);

		const Type *type() const override;

		const BaseValue *subject() const;

	private:
		const BaseValue *m_subject;
};

#endif // TAINTEDVALUE_H
