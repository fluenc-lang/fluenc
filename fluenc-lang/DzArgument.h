#ifndef DZARGUMENT_H
#define DZARGUMENT_H

#include <string>

#include "DzBaseArgument.h"

class ITypeName;
class EntryPoint;
class Type;

class DzArgument : public DzBaseArgument
{
	public:
		DzArgument(const std::string &name, ITypeName *type);

		std::string name() const;

		Type *type(const EntryPoint &entryPoint) const override;

	private:
		std::string m_name;

		ITypeName *m_type;
};

#endif // DZARGUMENT_H
