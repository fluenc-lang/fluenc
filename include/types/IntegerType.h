#ifndef INTEGERTYPE_H
#define INTEGERTYPE_H

#include "BuiltinType.h"
#include "IntegerOperatorSet.h"

template<typename T>
class IntegerType : public BuiltinType<T>
{
	public:
		IOperatorSet *operators() const override
		{
			static IntegerOperatorSet operators;

			return &operators;
		}
};

#endif // INTEGERTYPE_H
