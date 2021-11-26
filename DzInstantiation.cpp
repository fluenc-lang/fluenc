#include <llvm/IR/IRBuilder.h>

#include <sstream>

#include "DzInstantiation.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"

#include "types/Prototype.h"
#include "types/UserType.h"

#include "values/TypedValue.h"
#include "values/NamedValue.h"
#include "values/UserTypeValue.h"

DzInstantiation::DzInstantiation(DzValue *consumer
	, IPrototypeProvider *prototypeProvider
	, const std::unordered_map<std::string, DzValue *> &fields
	)
	: m_consumer(consumer)
	, m_prototypeProvider(prototypeProvider)
	, m_fields(fields)
{
}

std::vector<DzResult> DzInstantiation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto prototype = m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_insert_iterator(namedValues), [&](auto field) -> const NamedValue *
	{
		auto value = m_fields.find(field->name());

		if (value != m_fields.end())
		{
			return new NamedValue { field->name(), entryPoint, value->second, nullptr };
		}

		if (!field->hasValue())
		{
			throw new std::exception();
		}

		return field;
	});

	auto userTypeValue = new UserTypeValue(prototype, namedValues);

	values.push(userTypeValue);

	return m_consumer->build(entryPoint, values);
}
