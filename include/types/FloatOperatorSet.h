#ifndef FLOATOPERATORSET_H
#define FLOATOPERATORSET_H

#include "types/IOperatorSet.h"

class FloatOperatorSet : public IOperatorSet
{
	public:
		const ScalarValue *resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const override;
};

#endif // FLOATOPERATORSET_H
