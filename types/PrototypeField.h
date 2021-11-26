#ifndef PROTOTYPEFIELD_H
#define PROTOTYPEFIELD_H

#include <string>

class DzValue;
class DzTypeName;

class PrototypeField
{
	public:
		PrototypeField(const std::string &name
			, const DzValue *defaultValue
			, const DzTypeName *type
			);

		std::string name() const;

		const DzValue *defaultValue() const;
		const DzTypeName *type() const;

	private:
		std::string m_name;

		const DzValue *m_defaultValue;
		const DzTypeName *m_type;
};

#endif // PROTOTYPEFIELD_H
