#ifndef DZVALUE_H
#define DZVALUE_H

#include "NodeVisitor.h"
#include "Emitter.h"

class Node : public Visitable<Emitter>
{
	public:
		Node();

		size_t id() const;

		virtual int order(const EntryPoint &entryPoint) const;

	private:
		size_t m_id;
};

#endif // DZVALUE_H
