#ifndef DZCHARACTERLITERAL_H
#define DZCHARACTERLITERAL_H

#include "DzValue.h"

class DzCharacterLiteral : public DzValue
{
	public:
		DzCharacterLiteral(DzValue *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZCHARACTERLITERAL_H
