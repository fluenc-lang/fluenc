#ifndef DZMEMBERACCESS_H
#define DZMEMBERACCESS_H

#include "DzValue.h"

class DzMemberAccess : public DzValue
{
	public:
		DzMemberAccess(antlr4::ParserRuleContext *context, DzValue *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		antlr4::ParserRuleContext *m_context;

		DzValue * m_consumer;

		std::string m_name;
};

#endif // DZMEMBERACCESS_H
