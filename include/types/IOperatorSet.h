#ifndef IOPERATORSET_H
#define IOPERATORSET_H

#include <string>

class Analyzer;
class ScalarValue;
class IRBuilderEx;
class Node;

struct BinaryNode;
struct UnaryNode;

class IOperatorSet
{
	public:
		virtual const Node *forBinary(const BinaryNode *node) const = 0;
		virtual const Node *forUnary(const UnaryNode *node) const = 0;
};

#endif // IOPERATORSET_H
