#ifndef IOPERATORSET_H
#define IOPERATORSET_H

#include <string>

class Analyzer;
class ScalarValue;
class IRBuilderEx;
class BinaryNode;
class Node;

class IOperatorSet
{
	public:
		virtual const Node *forBinary(const BinaryNode *node) const = 0;
};

#endif // IOPERATORSET_H
