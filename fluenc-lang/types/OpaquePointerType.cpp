#include "OpaquePointerType.h"
#include "IPrototype.h"

#include <unordered_map>

OpaquePointerType::OpaquePointerType(const IPrototype *subject)
	: m_subject(subject)
{
}

TypeId OpaquePointerType::id() const
{
	return TypeId::OpaquePointer;
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

OpaquePointerType *OpaquePointerType::get(const IPrototype *subject)
{
	static std::unordered_map<const IPrototype *, OpaquePointerType> cache;

	auto [iterator, _] = cache.try_emplace(subject, subject);

	return &iterator->second;
}
