#include <unordered_map>

#include <llvm/IR/Constants.h>

#include "nodes/CharacterLiteralNode.h"

#include "values/ScalarValue.h"

#include "types/Int32Type.h"

CharacterLiteralNode::CharacterLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> CharacterLiteralNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = Int32Type::instance();
	auto storageType = type->storageType(*context);

	auto value = new ScalarValue(type
		, getValue(storageType)
		);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}

llvm::ConstantInt *CharacterLiteralNode::getValue(llvm::Type *storageType) const
{
	static std::unordered_map<std::string, std::string> sequences =
	{
		{ "\\n", "\n" },
		{ "\\r", "\r" },
		{ "\\t", "\t" },
		{ "\\0", "\0" },
	};

	auto iterator = sequences.find(m_value);

	if (iterator != sequences.end())
	{
		return llvm::ConstantInt::get((llvm::IntegerType *)storageType
			, *iterator->second.begin()
			);
	}

	return llvm::ConstantInt::get((llvm::IntegerType *)storageType
		, *m_value.begin()
		);
}
