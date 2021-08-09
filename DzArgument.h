#ifndef DZARGUMENT_H
#define DZARGUMENT_H

#include <string>

class DzTypeName;

class DzArgument
{
	public:
		DzArgument(const std::string &name, DzTypeName *type);

		std::string name() const;

		DzTypeName *type() const;

	private:
		std::string m_name;

		DzTypeName *m_type;
};

#endif // DZARGUMENT_H
