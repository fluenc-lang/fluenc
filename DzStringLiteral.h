#ifndef DZSTRINGLITERAL_H
#define DZSTRINGLITERAL_H

#include "DzValue.h"

class DzStringLiteral : public DzValue
{
	public:
		DzStringLiteral(DzValue *consumer, const std::string &value);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZSTRINGLITERAL_H
