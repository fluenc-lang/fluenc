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

		TypeKind kind() const override
		{
			return TypeKind::Builtin;
		}

		Type *iteratorType() const override
		{
			return IteratorType<T>::instance();
		}

		bool is(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			return tag() == type->tag();
		}
};

#endif // BUILTINTYPE_H
