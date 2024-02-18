#include "iterator_type.hpp"

namespace fluenc::code_generation::types
{
	base_type* iterator_type::instance()
	{
		static iterator_type instance;

		return &instance;
	}

	type_id iterator_type::id() const
	{
		return type_id::iterator;
	}

	std::string iterator_type::name() const
	{
		return "...";
	}

	llvm::Type* iterator_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt8PtrTy(context);
	}

	base_type* iterator_type::get(uint64_t it)
	{
		static std::unordered_map<uint64_t, iterator_type> cache;

		return &cache[it];
	}
}
