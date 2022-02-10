#include "DzIteratorFunction.h"
#include "DzBaseArgument.h"
#include "AllIterator.h"
#include "DzFunction.h"

#include "values/LazyValue.h"

DzIteratorFunction::DzIteratorFunction(const std::vector<DzBaseArgument *> &arguments
	, const Type *iteratorType
	, const DzFunction *subject
	)
	: m_arguments(arguments)
	, m_iteratorType(iteratorType)
	, m_subject(subject)
{
}

std::vector<DzResult> DzIteratorFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();
	auto function = entryPoint.function();

	block->insertInto(function);

	std::vector<const BaseValue *> arguments;

	for (auto i = 0u; i < m_arguments.size(); i++)
	{
		arguments.insert(begin(arguments), values.pop());
	}

	auto lazy = new LazyValue(arguments
		, entryPoint
		, m_iteratorType
		, m_subject
		);

	values.push(lazy);

	return {{ entryPoint, values }};
}

std::string DzIteratorFunction::name() const
{
	return m_subject->name();
}

FunctionAttribute DzIteratorFunction::attribute() const
{
	return FunctionAttribute::Iterator;
}

bool DzIteratorFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
{
	return m_subject->hasMatchingSignature(entryPoint, values);
}
