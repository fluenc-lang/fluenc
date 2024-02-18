#include "OpaquePointerType.h"
#include "structure_type.hpp"

#include <unordered_map>

opaque_pointer_type::opaque_pointer_type(const structure_type *subject)
	: m_subject(subject)
{
}

type_id opaque_pointer_type::id() const
{
    return type_id::OpaquePointer;
}

std::string opaque_pointer_type::name() const
{
	return m_subject->name();
}

std::string opaque_pointer_type::fullName() const
{
	return m_subject->fullName();
}

const structure_type *opaque_pointer_type::subject() const
{
	return m_subject;
}

llvm::Type *opaque_pointer_type::storageType(llvm::LLVMContext &context) const
{
	return m_subject->storageType(context);
}

opaque_pointer_type *opaque_pointer_type::get(const structure_type *subject)
{
	static std::unordered_map<const structure_type *, opaque_pointer_type> cache;

	auto [iterator, _] = cache.try_emplace(subject, subject);

	return &iterator->second;
}
