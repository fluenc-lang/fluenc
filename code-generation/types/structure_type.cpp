#include "structure_type.hpp"
#include "entry_point.hpp"

#include "ast/struct_node.hpp"

#include <unordered_map>

namespace fluenc::code_generation::types
{
	structure_type::structure_type(const struct_node* subject)
		: subject_(subject)
	{
	}

	type_id structure_type::id() const
	{
		return type_id::default_prototype;
	}

	std::string structure_type::name() const
	{
		return subject_->tag;
	}

	llvm::Type* structure_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::PointerType::get(context, 0);
	}

	std::vector<structure_field> structure_type::fields(const entry_point& type_resolver) const
	{
		std::vector<structure_field> results;

		for (auto& field : subject_->fields)
		{
			results.push_back({ field.name, type_for(field.type, type_resolver) });
		}

		return results;
	}

	const struct_node* structure_type::subject() const
	{
		return subject_;
	}

	structure_type* structure_type::get(const struct_node* subject)
	{
		static std::unordered_map<const struct_node*, structure_type> cache;

		auto [iterator, _] = cache.try_emplace(subject, subject);

		return &iterator->second;
	}
}
