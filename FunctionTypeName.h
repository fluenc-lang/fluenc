#ifndef FUNCTIONTYPENAME_H
#define FUNCTIONTYPENAME_H

#include <vector>

#include "ITypeName.h"

class Type;

class FunctionTypeName : public ITypeName
{
	public:
		FunctionTypeName(const std::vector<const ITypeName *> &types);

		Type *resolve(const EntryPoint &entryPoint) const override;

	private:
		std::vector<const ITypeName *> m_types;
};

#endif // FUNCTIONTYPENAME_H
