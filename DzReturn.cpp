#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzReturn.h"
#include "EntryPoint.h"
#include "Type.h"
#include "DzExportedFunctionTerminator.h"
#include "DzTerminator.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/DependentValue.h"

DzReturn::DzReturn(DzValue *consumer, DzValue *chained)
	: m_consumer(consumer)
	, m_chained(chained)
{
}

std::vector<DzResult> DzReturn::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->insertInto(function);

	llvm::IRBuilder<> builder(block);

	Stack localValues;

	if (m_chained)
	{
		localValues.push(new DependentValue { IteratorType::instance(), new EntryPoint(entryPoint), m_chained });
	}

	for (auto i = values.begin(); i < values.end(); i++)
	{
		auto value = *i;

		if (auto typedValue = dynamic_cast<const TypedValue *>(value))
		{
			auto type = value->type();
			auto storageType = type->storageType(*context);

			auto alloc = entryPoint.alloc(storageType);

			builder.CreateStore(*typedValue, alloc);

			auto load = builder.CreateLoad(storageType, alloc);

			localValues.push(new TypedValue { type, load });
		}
		else
		{
			localValues.push(value);
		}
	}

	return m_consumer->build(entryPoint, localValues);
}
