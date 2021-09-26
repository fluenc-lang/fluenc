#ifndef PROTOTYPEFIELD_H
#define PROTOTYPEFIELD_H

#include <string>

class DzValue;

class PrototypeField
{
	public:
		PrototypeField(const std::string &name, DzValue *defaultValue);

		std::string name() const;

		DzValue *defaultValue() const;

	private:
		std::string m_name;

		DzValue *m_defaultValue;
};

#endif // PROTOTYPEFIELD_H
