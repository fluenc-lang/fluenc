#ifndef USERTYPEFIELD_H
#define USERTYPEFIELD_H

#include <string>

class Type;

class UserTypeField
{
	public:
		UserTypeField(size_t index, const std::string &name, const Type *type);

		size_t index() const;

		std::string name() const;

		const Type *type() const;

	private:
		size_t m_index;

		std::string m_name;

		const Type *m_type;
};

#endif // USERTYPEFIELD_H
