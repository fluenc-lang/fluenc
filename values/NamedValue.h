#ifndef NAMEDVALUE_H
#define NAMEDVALUE_H

#include "BaseValue.h"

class DzValue;

class NamedValue : public BaseValue
{
	public:
		NamedValue(const std::string &name, const EntryPoint &entryPoint, const DzValue *subject);

		std::string name() const;

		const EntryPoint *entryPoint() const;
		const DzValue *subject() const;

		const Type *type() const override;

	private:
		std::string m_name;

		const EntryPoint *m_entryPoint;
		const DzValue *m_subject;
};

#endif // NAMEDVALUE_H
