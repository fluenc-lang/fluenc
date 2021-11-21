#include "NamedValue.h"
#include "EntryPoint.h"

NamedValue::NamedValue(const std::string &name, const EntryPoint &entryPoint, const DzValue *subject)
	: m_name(name)
	, m_entryPoint(new EntryPoint(entryPoint))
	, m_subject(subject)
{
}

std::string NamedValue::name() const
{
	return m_name;
}

std::vector<DzResult> NamedValue::build(llvm::BasicBlock *block, const Stack &values) const
{
	auto entryPoint = m_entryPoint->withBlock(block);

	return m_subject->build(entryPoint, values);
}

const Type *NamedValue::type() const
{
	throw new std::exception();
}

bool NamedValue::hasValue() const
{
	return m_subject;
}
