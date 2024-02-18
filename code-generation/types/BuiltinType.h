#ifndef BUILTINTYPE_H
#define BUILTINTYPE_H

#include "Type.h"

class IBuiltinType : public Type
{
};

template<typename T>
class BuiltinType : public IBuiltinType
{
	public:
        type_id id() const override
		{
            return type_id::Builtin;
		}

		static Type *instance()
		{
			static T instance;

			return &instance;
		}
};

#endif // BUILTINTYPE_H
