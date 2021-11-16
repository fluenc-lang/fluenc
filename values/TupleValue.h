#ifndef TUPLEVALUE_H
#define TUPLEVALUE_H

#include <sstream>
#include <numeric>

#include "BaseValue.h"
#include "Utility.h"

class TupleType : public Type
{
	public:
		TupleType(const std::vector<const Type *> types)
			: m_types(types)
		{
		}

		TypeKind kind() const override
		{
			return TypeKind::Iterator;
		}

		std::string tag() const override
		{
			std::stringstream ss;

			std::transform(begin(m_types), end(m_types), std::ostream_iterator<std::string>(ss), [](auto type)
			{
				return type->tag();
			});

			return ss.str();
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			UNUSED(context);

			return nullptr;
		}

		Type *iteratorType() const override
		{
			return nullptr;
		}

		bool is(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			return tag() == type->tag();
		}

		static TupleType *get(const std::vector<const Type *> types)
		{
			static std::unordered_map<size_t, TupleType> cache;

			auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
			{
				return (accumulator ^ (size_t)type) * 1099511628211;
			});

			auto [iterator, _] = cache.try_emplace(hash, types);

			return &iterator->second;
		}

	private:
		std::vector<const Type *> m_types;
};

class TupleValue : public BaseValue
{
	public:
		TupleValue(const std::vector<const BaseValue *> &values);

		const Type *type() const override
		{
			std::vector<const Type *> types;

			std::transform(begin(m_values), end(m_values), std::back_insert_iterator(types), [](auto value)
			{
				return value->type();
			});

			return TupleType::get(types);
		}

		std::vector<const BaseValue *> values() const
		{
			return m_values;
		}

	private:
		std::vector<const BaseValue *> m_values;
};

#endif // TUPLEVALUE_H
