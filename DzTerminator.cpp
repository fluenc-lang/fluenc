#include "DzTerminator.h"
#include "VisitorV2.h"

DzTerminator::DzTerminator()
	: m_value(nullptr)
{
}

DzValue *DzTerminator::value() const
{
	return m_value;
}

void DzTerminator::setValue(DzValue *value)
{
	m_value = value;
}

llvm::Value *DzTerminator::build(const EntryPoint &entryPoint) const
{
	return m_value->build(entryPoint);
}
