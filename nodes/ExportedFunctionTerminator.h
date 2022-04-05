#ifndef EXPORTEDFUNCTIONTERMINATOR_H
#define EXPORTEDFUNCTIONTERMINATOR_H

#include "Node.h"

class ExportedFunctionTerminator : public Node
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // EXPORTEDFUNCTIONTERMINATOR_H
