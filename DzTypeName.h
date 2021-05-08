#ifndef DZTYPENAME_H
#define DZTYPENAME_H

#include "DzValue.h"

class DzTypeName
{
	public:
		DzTypeName(std::string typeName);

		std::string name() const;

		llvm::Type *type(const EntryPoint &entryPoint) const;

	private:
		std::string m_typeName;
};

#endif // DZTYPENAME_H
