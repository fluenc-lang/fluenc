#ifndef PROTOTYPEFIELD_H
#define PROTOTYPEFIELD_H

#include <string>

class BaseValue;
class Type;

class PrototypeField
{
	public:
		PrototypeField(const std::string &name
			, const BaseValue *defaultValue
			, const Type *type
			);

		std::string name() const;

		const BaseValue *defaultValue() const;
		const Type *type() const;

	private:
		std::string m_name;

		const BaseValue *m_defaultValue;
		const Type *m_type;
};

#endif // PROTOTYPEFIELD_H
