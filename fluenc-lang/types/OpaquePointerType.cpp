#include "OpaquePointerType.h"
#include "IPrototype.h"

OpaquePointerType::OpaquePointerType(const IPrototype *subject)
	: m_subject(subject)
{
}

std::string OpaquePointerType::name() const
{
	return m_subject->name();
}

std::string OpaquePointerType::fullName() const
{
	return m_subject->fullName();
}

const IPrototype *OpaquePointerType::subject() const
{
	return m_subject;
}

llvm::Type *OpaquePointerType::storageType(llvm::LLVMContext &context) const
{
	return m_subject->storageType(context);
}
