#ifndef DZVALUE_H
#define DZVALUE_H

#include "NodeVisitor.h"
#include "Emitter.h"
#include "Analyzer.h"
#include "Visitable.h"

class Node : public Visitable<Emitter, Analyzer>
{
	public:
		Node();

		size_t id() const;

		virtual int order(const EntryPoint &entryPoint) const;

	private:
		size_t m_id;
};

template<typename TSelf>
class VisitableNode : public VisitableExpansion<Node, TSelf, typename Node::VisitorType>
{
};

#endif // DZVALUE_H
