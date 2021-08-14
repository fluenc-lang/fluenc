#ifndef DZINTEGRALLITERAL_H
#define DZINTEGRALLITERAL_H

#include "DzValue.h"

class DzTypeName;

class DzIntegralLiteral : public DzValue
{
	public:
		DzIntegralLiteral(DzValue *consumer, DzTypeName *type, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
		DzTypeName *m_type;

		std::string m_value;
};

#endif // DZINTEGRALLITERAL_H
