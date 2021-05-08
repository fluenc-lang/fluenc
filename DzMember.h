#ifndef DZMEMBER_H
#define DZMEMBER_H

#include <string>

class DzTypeName;

class DzMember
{
	public:
		DzMember(const std::string &name, DzTypeName *type);

		std::string name() const;

		DzTypeName *type() const;

	private:
		std::string m_name;

		DzTypeName *m_type;
};

#endif // DZMEMBER_H
