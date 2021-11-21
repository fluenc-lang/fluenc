#ifndef NAMEDVALUE_H
#define NAMEDVALUE_H

#include "BaseValue.h"

class DzValue;

class NamedValue
{
	public:
		NamedValue(const std::string &name, const EntryPoint &entryPoint, DzValue *subject);

		std::string name() const;

		const EntryPoint *entryPoint() const;
		DzValue *subject() const;

	private:
		std::string m_name;

		const EntryPoint *m_entryPoint;
		DzValue *m_subject;
};

#endif // NAMEDVALUE_H
