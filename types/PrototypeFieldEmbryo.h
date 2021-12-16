#ifndef PROTOTYPEFIELDEMBRYO_H
#define PROTOTYPEFIELDEMBRYO_H

#include <string>

class DzValue;
class DzTypeName;
class Type;
class EntryPoint;

class PrototypeFieldEmbryo
{
	public:
		PrototypeFieldEmbryo(const std::string &name
			, const DzValue *defaultValue
			, const DzTypeName *type
			);

		std::string name() const;

		const DzValue *defaultValue() const;
		const Type *type(const EntryPoint &entryPoint) const;

	private:
		std::string m_name;

		const DzValue *m_defaultValue;
		const DzTypeName *m_type;
};

#endif // PROTOTYPEFIELDEMBRYO_H
