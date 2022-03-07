#ifndef PROTOTYPEFIELDEMBRYO_H
#define PROTOTYPEFIELDEMBRYO_H

#include <string>

class DzValue;
class ITypeName;
class Type;
class EntryPoint;

class PrototypeFieldEmbryo
{
	public:
		PrototypeFieldEmbryo(const std::string &name
			, const DzValue *defaultValue
			, const ITypeName *type
			);

		std::string name() const;

		const DzValue *defaultValue() const;
		const Type *type(const EntryPoint &entryPoint) const;

	private:
		std::string m_name;

		const DzValue *m_defaultValue;
		const ITypeName *m_type;
};

#endif // PROTOTYPEFIELDEMBRYO_H
