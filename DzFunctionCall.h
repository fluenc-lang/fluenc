#ifndef DZFUNCTIONCALL_H
#define DZFUNCTIONCALL_H

#include "DzValue.h"

class DzFunctionCall : public DzValue
{
	public:
		DzFunctionCall(DzValue *consumer
			, const std::string name
			);

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		DzValue *m_consumer;

		std::string m_name;
};

#endif // DZFUNCTIONCALL_H
