#ifndef DZMEMBERACCESS_H
#define DZMEMBERACCESS_H

#include "DzValue.h"

class DzMemberAccess : public DzValue
{
	public:
		DzMemberAccess(DzValue *consumer, const std::string &name);

		Stack build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;

		std::string m_name;
};

#endif // DZMEMBERACCESS_H
