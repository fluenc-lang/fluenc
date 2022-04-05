#ifndef DZARRAYELEMENT_H
#define DZARRAYELEMENT_H

#include "Node.h"

class Type;

class ArrayElement : public Node
{
	public:
		ArrayElement(const Type *iteratorType, const Node *next);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_next;
};
#endif // DZARRAYELEMENT_H
