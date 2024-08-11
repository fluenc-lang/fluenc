#include "opaque_pointer_type.hpp"
#include "structure_type.hpp"

#include <unordered_map>

namespace fluenc::code_generation::types
{
	opaque_pointer_type::opaque_pointer_type(const structure_type* subject)
		: subject_(subject)
	{
	}

	type_id opaque_pointer_type::id() const
	{
		return type_id::opaque_pointer;
	}

	std::string opaque_pointer_type::name() const
	{
		return subject_->name();
	}

	std::string opaque_pointer_type::full_name() const
	{
		return subject_->full_name();
	}

	const structure_type* opaque_pointer_type::subject() const
	{
		return subject_;
	}

	llvm::Type* opaque_pointer_type::storage_type(llvm::LLVMContext& context) const
	{
		return subject_->storage_type(context);
	}

	opaque_pointer_type* opaque_pointer_type::get(const structure_type* subject)
	{
		static std::unordered_map<const structure_type*, opaque_pointer_type> cache;

		auto [iterator, _] = cache.try_emplace(subject, subject);

		return &iterator->second;
	}
}
