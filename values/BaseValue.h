#ifndef BASEVALUE_H
#define BASEVALUE_H

#include "Type.h"

class BaseValue
{
	public:
		virtual ~BaseValue() = default;

		virtual const Type *type() const = 0;

//		virtual std::string tag() const = 0;
};

#endif // BASEVALUE_H
