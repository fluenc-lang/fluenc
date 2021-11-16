#ifndef DZTUPLEARGUMENT_H
#define DZTUPLEARGUMENT_H

#include <vector>

#include "DzBaseArgument.h"

class DzTupleArgument : public DzBaseArgument
{
	public:
		DzTupleArgument(const std::vector<DzBaseArgument *> &arguments);

		std::vector<DzBaseArgument *> arguments() const;

		Type *type(const EntryPoint &entryPoint) const override;

	private:
		std::vector<DzBaseArgument *> m_arguments;
};


#endif // DZTUPLEARGUMENT_H
