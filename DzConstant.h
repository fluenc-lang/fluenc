#ifndef DZCONSTANT_H
#define DZCONSTANT_H

#include "DzValue.h"

class DzConstant : public DzValue
{
	public:
		DzConstant(DzValue *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZCONSTANT_H
