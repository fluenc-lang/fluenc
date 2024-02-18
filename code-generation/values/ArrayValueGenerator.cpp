#include <numeric>

#include <llvm/IR/Constants.h>

#include "ScalarValue.h"
#include "IRBuilderEx.h"

#include "iterators/index_iterator.hpp"

#include "values/ArrayValueGenerator.h"
#include "values/ReferenceValue.h"
#include "values/IndexedValue.h"
#include "values/Iteratable.h"

#include "fragments/ArrayValue.h"
#include "fragments/ArrayValueProxy.h"
#include "fragments/ArrayElementNode.h"

#include "types/ArrayType.h"
#include "types/IteratorType.h"
#include "types/Int64Type.h"

ArrayValueGenerator::ArrayValueGenerator(const std::vector<DzResult > &values, const std::shared_ptr<peg::Ast> &ast, size_t size)
	: m_values(values)
	, m_ast(ast)
	, m_size(size)
{
}

const fragment *ArrayValueGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(mode);

	IRBuilderEx builder(entryPoint);

	auto context = entryPoint.context();

        auto indexType = int64_type::instance();

	auto storageType = indexType->storageType(*context);

        auto zero = new scalar_value(indexType
		, llvm::ConstantInt::get(storageType, 0)
		);

	auto index = entryPoint.alloc(indexType, "index");

	builder.createStore(zero, index);

	auto subject = new ArrayValueProxy(index
		, new ArrayValue(m_ast, index, type(), m_values, m_size)
		);

	return new Iteratable(subject, type());
}

const Type *ArrayValueGenerator::type() const
{
	if (m_values.size() == 1)
	{
		auto [_, values] = *m_values.begin();

		std::vector<const Type *> types;

		std::transform(values.rbegin(), values.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

                return array_type::get(types);
	}

        return iterator_type::instance();
}

size_t ArrayValueGenerator::size() const
{
	return m_size;
}

const BaseValue *ArrayValueGenerator::elementAt(size_t index) const
{
	if (m_values.size() != 1)
	{
		throw std::exception();
	}

	auto [_, values] = m_values[0];

	if (index >= values.size())
	{
		throw std::exception();
	}

        auto indexed = value_cast<const indexed_value *>(*(values.rbegin() + index));

	return indexed->subject();
}

const ILazyValueGenerator *ArrayValueGenerator::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	if (m_values.size() == 1)
	{
		auto [inputEntryPoint, inputValues] = m_values[0];

		std::vector<const BaseValue *> clonedValues;

		std::transform(inputValues.begin(), inputValues.end(), std::back_inserter(clonedValues), [&](auto value)
		{
			return value->clone(entryPoint, strategy);
		});

		return new ArrayValueGenerator({{ inputEntryPoint, clonedValues }}, m_ast, m_size);
	}

	throw std::exception();
}
