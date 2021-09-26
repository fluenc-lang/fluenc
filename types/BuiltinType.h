#ifndef BUILTINTYPE_H
#define BUILTINTYPE_H

#include "Type.h"

template<typename T>
class BuiltinType : public Type
{
	public:
		static Type *instance()
		{
			static T instance;

			return &instance;
		}
};

#endif // BUILTINTYPE_H
