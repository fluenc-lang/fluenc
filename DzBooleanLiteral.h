#ifndef DZBOOLEANLITERAL_H
#define DZBOOLEANLITERAL_H

#include "DzValue.h"

class DzBooleanLiteral : public DzValue
{
	public:
		DzBooleanLiteral(DzValue *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		TypedValue resolveValue(const EntryPoint &entryPoint) const;

		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZBOOLEANLITERAL_H
