#ifndef DZFUNCTIONTERMINATOR_H
#define DZFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzFunctionTerminator : public DzValue
{
	public:
		DzFunctionTerminator(const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
};

#endif // DZFUNCTIONTERMINATOR_H
