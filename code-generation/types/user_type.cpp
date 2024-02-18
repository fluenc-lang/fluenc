#include <numeric>
#include <sstream>
#include <unordered_map>

#include "operator_set.hpp"
#include "structure_type.hpp"
#include "user_operator_set.hpp"
#include "user_type.hpp"

namespace fluenc::code_generation::types
{
	user_type::user_type(const structure_type* prototype, const std::vector<const base_type*>& elementTypes)
		: prototype_(prototype)
		, element_types_(elementTypes)
	{
	}

	type_id user_type::id() const
	{
		return type_id::user;
	}

	std::string user_type::name() const
	{
		return prototype_->name();
	}

	std::string user_type::full_name() const
	{
		std::ostringstream stream;

		stream << prototype_->name();

		if (element_types_.size() > 0)
		{
			stream << "<";

			for (auto i = begin(element_types_); i != end(element_types_); i++)
			{
				if (i != begin(element_types_))
				{
					stream << ", ";
				}

				stream << (*i)->full_name();
			}

			stream << ">";
		}

		return stream.str();
	}

	llvm::Type* user_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt8PtrTy(context);
	}

	const structure_type* user_type::prototype() const
	{
		return prototype_;
	}

	operator_set* user_type::operators() const
	{
		static user_operator_set operators;

		return &operators;
	}

	std::vector<const base_type*> user_type::element_types() const
	{
		return element_types_;
	}

	user_type* user_type::get(const structure_type* prototype, const std::vector<const base_type*>& elementTypes)
	{
		static std::unordered_map<size_t, user_type> cache;

		auto hash = std::accumulate(
			begin(elementTypes),
			end(elementTypes),
			(14695981039346656037u ^ reinterpret_cast<size_t>(prototype)) * 1099511628211,
			[](auto accumulator, auto type) {
				return (accumulator ^ reinterpret_cast<size_t>(type)) * 1099511628211;
			}
		);

		auto [iterator, _] = cache.try_emplace(hash, user_type { prototype, elementTypes });

		return &iterator->second;
	}
}
