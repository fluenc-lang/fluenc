#include <sstream>
#include <numeric>
#include <unordered_map>

#include "IPrototype.h"
#include "UserType.h"
#include "IOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

UserType::UserType(const IPrototype *prototype, const std::vector<const Type *> &elementTypes)
	: m_prototype(prototype)
	, m_elementTypes(elementTypes)
{
}

TypeId UserType::id() const
{
	return TypeId::User;
}

std::string UserType::name() const
{
	return m_prototype->name();
}

std::string UserType::fullName() const
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

llvm::Type *UserType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

const IPrototype *UserType::prototype() const
{
	return m_prototype;
}

class UserOperatorSet : public IOperatorSet
{
	const Node *forBinary(const BinaryNode *node) const
	{
		auto result = new UserBinaryNode();
		result->ast = node->ast;
		result->consumer = node->consumer;
		result->op = node->op;

		return result;
	}

	const Node *forUnary(const UnaryNode *node) const
	{
		auto result = new UserUnaryNode();
		result->ast = node->ast;
		result->consumer = node->consumer;
		result->op = node->op;

		return result;
	}
};

IOperatorSet *UserType::operators() const
{
	static UserOperatorSet operators;

	return &operators;
}

std::vector<const Type *> UserType::elementTypes() const
{
	return m_elementTypes;
}

UserType *UserType::get(const IPrototype *prototype, const std::vector<const Type *> &elementTypes)
{
	static std::unordered_map<size_t, UserType> cache;

	auto hash = std::accumulate(begin(elementTypes), end(elementTypes)
		, (14695981039346656037u ^ (size_t)prototype) * 1099511628211
		, [](auto accumulator, auto type)
		{
			return (accumulator ^ (size_t)type) * 1099511628211;
		});

	auto [iterator, _] = cache.try_emplace(hash, UserType { prototype, elementTypes });

	return &iterator->second;
}
