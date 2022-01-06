#include "IteratorTypeHandle.h"

#include "types/IteratorType.h"

IteratorTypeHandle::IteratorTypeHandle()
	: m_type(new IteratorType)
{
}

const Type *IteratorTypeHandle::type() const
{
	return m_type;
}
