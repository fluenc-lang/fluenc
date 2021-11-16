#ifndef DZARGUMENT_H
#define DZARGUMENT_H

#include <string>

#include "DzBaseArgument.h"

class DzTypeName;
class EntryPoint;
class Type;

class DzArgument : public DzBaseArgument
{
	public:
		DzArgument(const std::string &name, DzTypeName *type);

		std::string name() const;

		Type *type(const EntryPoint &entryPoint) const override;

	private:
		std::string m_name;

		DzTypeName *m_type;
};

#endif // DZARGUMENT_H
