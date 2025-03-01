#include "string_operator_set.hpp"
#include "string_type.hpp"

namespace fluenc::code_generation::types
{
	string_type::string_type()
		: length_(-1)
	{
	}

	string_type::string_type(size_t length)
		: length_(length)
	{
	}

	type_id string_type::id() const
	{
		return type_id::string;
	}

	size_t string_type::length() const
	{
		return length_;
	}

	std::string string_type::name() const
	{
		return "string";
	}

	llvm::Type* string_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::PointerType::get(context, 0);
	}

	operator_set* string_type::operators() const
	{
		static string_operator_set operators;

		return &operators;
	}
}
