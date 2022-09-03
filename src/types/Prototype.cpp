#include <numeric>

#include "types/Prototype.h"
#include "DzTypeName.h"
#include "Node.h"
#include "types/AnyType.h"

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

std::vector<PrototypeField> Prototype::fields(const EntryPoint &entryPoint) const
{
	std::vector<PrototypeField> fields;

	std::transform(begin(m_fields), end(m_fields), std::back_insert_iterator(fields), [&](auto field) -> PrototypeField
	{
		auto defaultValue = field.defaultValue();

		if (defaultValue)
		{
			Emitter emitter;

			auto defaultResults = defaultValue->accept(emitter, { entryPoint, Stack() });

			auto &[_, defaultValues] = *defaultResults.begin();

			return { field.name(), defaultValues.pop(), field.type(entryPoint) };
		}

		return { field.name(), nullptr, field.type(entryPoint) };
	});

	for (auto type : m_parentTypes)
	{
		auto prototype = (Prototype *)type->resolve(entryPoint);
		auto parentFields = prototype->fields(entryPoint);

		fields.insert(end(fields), begin(parentFields), end(parentFields));
	}

	return fields;
}

llvm::Type *Prototype::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

int8_t Prototype::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	auto [match, score] = compatibility(1, type, entryPoint);

	if (match)
	{
		return score;
	}

	if (dynamic_cast<const AnyType *>(type))
	{
		return score + 1;
	}

	return -1;
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
