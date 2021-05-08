#ifndef CALLCONTEXT_H
#define CALLCONTEXT_H

#include <string>

#include "FunctionAttribute.h"

class DzValue;
class DzFunctionCall;

class CallContext
{
	public:
		CallContext(FunctionAttribute attribute
			, DzValue *root
			, DzFunctionCall *call
			);

		FunctionAttribute attribute() const;

		DzValue *root() const;
		DzFunctionCall *call() const;

	private:
		FunctionAttribute m_attribute;

		DzValue *m_root;
		DzFunctionCall *m_call;
};

#endif // CALLCONTEXT_H
