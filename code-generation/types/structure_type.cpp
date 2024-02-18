#include "structure_type.hpp"

#include "EntryPoint.h"

#include "ast/struct_node.hpp"

structure_type::structure_type(const fluenc::struct_node *subject)
	: m_subject(subject)
{
}

type_id structure_type::id() const
{
    return type_id::Prototype;
}

std::string structure_type::name() const
{
	return m_subject->tag;
}

llvm::Type *structure_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

std::vector<structure_field> structure_type::fields(const EntryPoint &type_resolver) const
{
	std::vector<structure_field> results;

	for (auto &field : m_subject->fields)
	{
		results.emplace_back(field.name, type_for(field.type, type_resolver));
	}

	return results;
}

const fluenc::struct_node *structure_type::subject() const
{
	return m_subject;
}

structure_type *structure_type::get(const fluenc::struct_node *subject)
{
	static std::unordered_map<const fluenc::struct_node*, structure_type> cache;

	auto [iterator, _] = cache.try_emplace(subject, subject);

	return &iterator->second;
}
