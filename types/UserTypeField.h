#ifndef USERTYPEFIELD_H
#define USERTYPEFIELD_H

#include <string>

class Type;

class UserTypeField
{
	public:
		UserTypeField(int index, const std::string &name, Type *type);

		int index() const;

		std::string name() const;

		Type *type() const;

	private:
		int m_index;

		std::string m_name;

		Type *m_type;
};

#endif // USERTYPEFIELD_H
