#ifndef PROTOTYPEFIELDEMBRYO_H
#define PROTOTYPEFIELDEMBRYO_H

#include <string>

class Node;
class ITypeName;
class Type;
class EntryPoint;

class PrototypeFieldEmbryo
{
	public:
		PrototypeFieldEmbryo(const std::string &name
			, const Node *defaultValue
			, const ITypeName *type
			);

		std::string name() const;

		const Node *defaultValue() const;
		const Type *type(const EntryPoint &entryPoint) const;

	private:
		std::string m_name;

		const Node *m_defaultValue;
		const ITypeName *m_type;
};

#endif // PROTOTYPEFIELDEMBRYO_H
