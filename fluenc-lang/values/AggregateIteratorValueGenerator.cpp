#include <llvm/IR/Constants.h>

#include "AggregateIteratorValueGenerator.h"
#include "AggregateIteratorValue.h"
#include "IRBuilderEx.h"
#include "Utility.h"

#include "types/ArrayType.h"
#include "types/WithoutType.h"
#include "types/ArrayType.h"
#include "types/Int64Type.h"
#include "types/TupleType.h"

#include "values/ScalarValue.h"

AggregateIteratorValueGenerator::AggregateIteratorValueGenerator(const ArrayBinaryNode *node, const std::vector<const BaseValue *> &input)
	: m_node(node)
	, m_input(input)
{
}

const IIteratable *AggregateIteratorValueGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(mode);

	auto llvmContext = entryPoint.context();

	auto indexType = Int64Type::instance();

	auto storageType = indexType->storageType(*llvmContext);

	auto zero = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 0)
		);

	auto index = entryPoint.alloc(indexType, "index");

	IRBuilderEx builder(entryPoint);

	builder.createStore(zero, index);

	return new AggregateIteratorValue(index, m_node, type(), m_input);
}

const ILazyValueGenerator *AggregateIteratorValueGenerator::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const Type *AggregateIteratorValueGenerator::type() const
{
	std::map<size_t, std::array<const Type *, 2>> map;

	for (auto i = 0u; i < m_input.size(); i++)
	{
		auto type = m_input[i]->type();

		if (auto array = type_cast<const ArrayType *>(type))
		{
			auto types = array->types();

			for (auto j = 0u; j < types.size(); j++)
			{
				map[j][i] = types[j];
			}
		}
		else
		{
			throw std::exception();
		}
	}

	std::vector<const Type *> elementTypes;

	std::transform(begin(map), end(map), back_inserter(elementTypes), [](auto pair)
	{
		auto [_, input] = pair;

		std::vector<const Type *> types;

		std::transform(begin(input), end(input), back_inserter(types), [](auto type) -> const Type *
		{
			if (!type)
			{
				return WithoutType::instance();
			}

			return type;
		});

		return TupleType::get(types);
	});

	return ArrayType::get(elementTypes);
}
