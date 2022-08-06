#ifndef FLOATOPERATORSET_H
#define FLOATOPERATORSET_H

#include "types/IOperatorSet.h"

class FloatOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const override;
};

#endif // FLOATOPERATORSET_H
