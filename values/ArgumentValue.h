#ifndef ARGUMENTVALUE_H
#define ARGUMENTVALUE_H

#include "BaseValue.h"

class TypedValue;

class ArgumentValue : public BaseValue
{
	public:
		ArgumentValue(int index, TypedValue *subject);

		int index() const;

		TypedValue *subject() const;

	private:
		int m_index;

		TypedValue *m_subject;
};

#endif // ARGUMENTVALUE_H
