#include <numeric>

#include "Prototype.h"
#include "ITypeName.h"
#include "Node.h"

Prototype::Prototype(const std::string &tag
	, const std::vector<PrototypeFieldEmbryo> &fields
	, const std::vector<ITypeName *> &parentTypes
	)
	: m_tag(tag)
	, m_fields(fields)
	, m_parentTypes(parentTypes)
{
}

const IPrototype *Prototype::root() const
{
	return this;
}

std::string Prototype::name() const
{
	return m_tag;
}

std::pair<EntryPoint, std::vector<PrototypeField>> Prototype::fields(const EntryPoint &entryPoint, const DefaultNodeVisitor &visitor) const
{
	std::vector<PrototypeField> fields;

	auto accumulatedEntryPoint = std::accumulate(begin(m_fields), end(m_fields), entryPoint, [&](auto currentEntryPoint, auto field) -> EntryPoint
	{
		auto defaultValue = field.defaultValue();

		if (defaultValue)
		{
			auto defaultResults = defaultValue->accept(visitor, { currentEntryPoint, Stack() });

			auto &[defaultEntryPoint, defaultValues] = *defaultResults.begin();

			fields.push_back({ field.name(), defaultValues.pop(), field.type(defaultEntryPoint) });

			return defaultEntryPoint;
		}
		else
		{
			fields.push_back({ field.name(), nullptr, field.type(currentEntryPoint) });

			return currentEntryPoint;
		}
	});

	auto finalEntryPoint = std::accumulate(begin(m_parentTypes), end(m_parentTypes), accumulatedEntryPoint, [&](auto aep, auto type)
	{
		auto prototype = (Prototype *)type->resolve(aep);
		auto [parentEntryPoint, parentFields] = prototype->fields(aep, visitor);

		fields.insert(end(fields), begin(parentFields), end(parentFields));

		return parentEntryPoint;
	});

	return { finalEntryPoint, fields };
}

std::vector<ITypeName *> Prototype::parentTypes() const
{
	return m_parentTypes;
}

llvm::Type *Prototype::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

std::pair<bool, int8_t> Prototype::compatibility(int8_t score, const Type *type, const EntryPoint &entryPoint) const
{
	if (type->name() == m_tag)
	{
		return { true, score };
	}

	return std::accumulate(begin(m_parentTypes), end(m_parentTypes), std::make_pair(false, score), [=](auto accumulated, auto parentType) -> std::pair<bool, int8_t>
	{
		auto resolvedType = parentType->resolve(entryPoint);
		auto prototype = static_cast<const Prototype *>(resolvedType);

		auto [accumulatedMatch, accumulatedScore] = accumulated;
		auto [currentMatch, currentScore] = prototype->compatibility(score + 1, type, entryPoint);

		if (!accumulatedMatch)
		{
			return { currentMatch, currentScore };
		}

		if (!currentMatch)
		{
			return { accumulatedMatch, accumulatedScore };
		}

		return { true, std::min(currentScore, accumulatedScore) };
	});
}
