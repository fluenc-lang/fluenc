#ifndef BUILTINTYPE_H
#define BUILTINTYPE_H

#include "Type.h"
#include "Utility.h"
#include "AnyType.h"

template<typename T>
class BuiltinType : public Type
{
	public:
		static Type *instance()
		{
			static T instance;

			return &instance;
		}

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			if (type == this)
			{
				return 0;
			}

			if (type->name() == name())
			{
				return 1;
			}

			if (dynamic_cast<const AnyType *>(type))
			{
				return 1;
			}

			return -1;
		}
};

#endif // BUILTINTYPE_H
