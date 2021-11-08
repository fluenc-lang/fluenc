#ifndef DZFUNCTIONCALL_H
#define DZFUNCTIONCALL_H

#include "DzValue.h"

class DzFunctionCall : public DzValue
{
	public:
		DzFunctionCall(const std::string name);

		bool compare(const DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
};

#endif // DZFUNCTIONCALL_H
