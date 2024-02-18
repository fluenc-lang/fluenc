#include <sstream>
#include <numeric>
#include <unordered_map>

#include "UserType.h"
#include "IOperatorSet.h"
#include "structure_type.hpp"

#include "ast/binary_node.hpp"
#include "ast/unary_node.hpp"

#include "fragments/binary_fragment.hpp"
#include "fragments/unary_fragment.hpp"

user_type::user_type(const structure_type *prototype, const std::vector<const Type *> &elementTypes)
	: m_prototype(prototype)
	, m_elementTypes(elementTypes)
{
}

type_id user_type::id() const
{
    return type_id::User;
}

std::string user_type::name() const
{
	return m_prototype->name();
}

std::string user_type::fullName() const
{
	std::ostringstream stream;

	stream << m_prototype->name();

	if (m_elementTypes.size() > 0)
	{
		stream << "<";

		for (auto i = begin(m_elementTypes)
			; i != end(m_elementTypes)
			; i++
			)
		{
			if (i != begin(m_elementTypes))
			{
				stream << ", ";
			}

			stream << (*i)->fullName();
		}

		stream << ">";
	}

	return stream.str();
}

llvm::Type *user_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

const structure_type *user_type::prototype() const
{
	return m_prototype;
}

class UserOperatorSet : public IOperatorSet
{
	fragment* forBinary(const fluenc::binary_node *node) const
	{
		auto result = new user_binary_fragment();
		result->binary = node;

		return result;
	}

	fragment* forUnary(const fluenc::unary_node *node) const
	{
		auto result = new user_unary_fragment();
		result->unary = node;

		return result;
	}
};

IOperatorSet *user_type::operators() const
{
	static UserOperatorSet operators;

	return &operators;
}

std::vector<const Type *> user_type::elementTypes() const
{
	return m_elementTypes;
}

user_type *user_type::get(const structure_type *prototype, const std::vector<const Type *> &elementTypes)
{
    static std::unordered_map<size_t, user_type> cache;

	auto hash = std::accumulate(begin(elementTypes), end(elementTypes)
		, (14695981039346656037u ^ (size_t)prototype) * 1099511628211
		, [](auto accumulator, auto type)
		{
			return (accumulator ^ (size_t)type) * 1099511628211;
		});

        auto [iterator, _] = cache.try_emplace(hash, user_type { prototype, elementTypes });

	return &iterator->second;
}
