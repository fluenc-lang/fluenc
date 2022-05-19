#ifndef BASEVALUE_H
#define BASEVALUE_H

#include "Type.h"
#include "Utility.h"

#include "metadata/ValueMetadata.h"

class BaseValue
{
	public:
		virtual ~BaseValue() = default;

		virtual const Type *type() const = 0;

		virtual const BaseValue *clone(const EntryPoint &entryPoint) const = 0;
		virtual const BaseValue *forward(size_t id) const
		{
			UNUSED(id);

			return this;
		}

		virtual const ValueMetadata &metadata() const = 0;
};

template<typename TMetadata>
struct BaseValueWithMetadata : public BaseValue
{
	static const ValueMetadata &staticMetadata()
	{
		static TMetadata instance;

		return instance;
	}

	const ValueMetadata &metadata() const override
	{
		return staticMetadata();
	}
};

#endif // BASEVALUE_H
