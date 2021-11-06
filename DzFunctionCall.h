#ifndef DZFUNCTIONCALL_H
#define DZFUNCTIONCALL_H

#include "DzValue.h"

class DzFunctionCall : public DzValue
{
	public:
		DzFunctionCall(const std::string name, size_t numberOfArguments);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		size_t m_numberOfArguments;
};

#endif // DZFUNCTIONCALL_H
