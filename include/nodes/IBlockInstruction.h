#ifndef IBLOCKINSTRUCTION_H
#define IBLOCKINSTRUCTION_H

#include "Node.h"

class IBlockInstruction : public Node
{
	public:
		virtual bool containsIterator() const = 0;
};

#endif // IBLOCKINSTRUCTION_H
