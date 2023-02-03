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

llvm::Type *OpaquePointerType::storageType(llvm::LLVMContext &context) const
{
	return m_subject->storageType(context);
}

int8_t OpaquePointerType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	if (auto other = dynamic_cast<const OpaquePointerType *>(type))
	{
		if (type->name() == name())
		{
			return 0;
		}
	}

	return m_subject->compatibility(type, entryPoint);
}
