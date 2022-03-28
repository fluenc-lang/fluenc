#include "DzValue.h"
#include "DzFunctionCall.h"

static size_t s_id;

DzValue::DzValue()
	: m_id(s_id++)
{
}

size_t DzValue::id() const
{
	return m_id;
}

int DzValue::order(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return -1;
}
