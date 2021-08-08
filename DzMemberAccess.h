#ifndef DZMEMBERACCESS_H
#define DZMEMBERACCESS_H

#include "DzValue.h"

class DzMemberAccess : public DzValue
{
	public:
		DzMemberAccess(DzValue *consumer, const std::string &name);

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		DzValue *m_consumer;

		std::string m_name;
};

#endif // DZMEMBERACCESS_H
