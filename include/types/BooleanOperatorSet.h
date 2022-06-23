#ifndef BOOLEANOPERATORSET_H
#define BOOLEANOPERATORSET_H

#include "types/IOperatorSet.h"

class BooleanOperatorSet : public IOperatorSet
{
	public:
		const ScalarValue *resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const override;
};

#endif // BOOLEANOPERATORSET_H
