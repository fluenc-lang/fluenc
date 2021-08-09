#ifndef DZFUNCTIONCALL_H
#define DZFUNCTIONCALL_H

#include "DzValue.h"

class DzFunctionCall : public DzValue
{
	public:
		DzFunctionCall(DzValue *consumer
			, const std::string name
			);

		Stack build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;

		std::string m_name;
};

#endif // DZFUNCTIONCALL_H
