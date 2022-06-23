#ifndef IOPERATORSET_H
#define IOPERATORSET_H

#include <string>

class ScalarValue;
class IRBuilderEx;

class IOperatorSet
{
	public:
		virtual const ScalarValue *resolve(const std::string &op, const IRBuilderEx &builder, const ScalarValue *left, const ScalarValue *right) const = 0;
};

#endif // IOPERATORSET_H
