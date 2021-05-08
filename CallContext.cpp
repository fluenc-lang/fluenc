#include "CallContext.h"

CallContext::CallContext(FunctionAttribute attribute, DzValue *root, DzFunctionCall *call)
	: m_attribute(attribute)
	, m_root(root)
	, m_call(call)
{
}

FunctionAttribute CallContext::attribute() const
{
	return m_attribute;
}

DzValue *CallContext::root() const
{
	return m_root;
}

DzFunctionCall *CallContext::call() const
{
	return m_call;
}
