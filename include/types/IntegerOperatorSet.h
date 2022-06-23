#ifndef INTEGEROPERATORSET_H
#define INTEGEROPERATORSET_H

#include "IOperatorSet.h"

class IntegerOperatorSet : public IOperatorSet
{
	public:
		const ScalarValue *resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const override;
};

#endif // INTEGEROPERATORSET_H
