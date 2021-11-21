#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

#include "DzMemberAccess.h"
#include "EntryPoint.h"
#include "Type.h"

#include "values/TypedValue.h"

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> DzMemberAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();
	auto locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		throw new std::exception();
	}

	if (auto computedValue = dynamic_cast<const TypedValue *>(iterator->second))
	{
		auto valueType = computedValue->type();

		auto storageType = valueType->storageType(*context);

		auto dataLayout = module->getDataLayout();
		auto align = dataLayout.getABITypeAlign(storageType);

		auto load = new llvm::LoadInst(storageType, *computedValue, m_name, false, align, block);

		values.push(new TypedValue { valueType, load });
	}
	else if (auto blast = dynamic_cast<const Blast *>(iterator->second))
	{
		std::vector<DzResult> res;

		auto ep = blast->entryPoint()
			->withBlock(block);

		auto k = blast->subject()->build(ep, values);

		for (auto &[epp, v] : k)
		{
			auto eep = entryPoint
				.withBlock(epp.block());

			for (auto &r : m_consumer->build(eep, v))
			{
				res.push_back(r);
			}
		}

		return res;
	}
	else if (iterator->second)
	{
		values.push(iterator->second);
	}

	return m_consumer->build(entryPoint, values);
}
