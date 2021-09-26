#ifndef USERTYPEFIELD_H
#define USERTYPEFIELD_H

#include <string>

class Type;

class UserTypeField
{
	public:
		UserTypeField(const std::string &name, Type *type);

		std::string name() const;

		Type *type() const;

	private:
		std::string m_name;

		Type *m_type;
};

#endif // USERTYPEFIELD_H
