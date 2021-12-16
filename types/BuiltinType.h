#ifndef BUILTINTYPE_H
#define BUILTINTYPE_H

#include "Type.h"
#include "Utility.h"
#include "IteratorType.h"

template<typename T>
class BuiltinType : public Type
{
	public:
		static Type *instance()
		{
			static T instance;

			return &instance;
		}

		bool is(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			return name() == type->name();
		}
};

#endif // BUILTINTYPE_H
