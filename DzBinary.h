#ifndef DZBINARY_H
#define DZBINARY_H

#include "DzValue.h"

class DzBinary : public DzValue
{
	public:
		DzBinary(DzValue *consumer, const std::string &op);

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		llvm::Value *resolveOp(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const;

		DzValue *m_consumer;

		std::string m_op;
};

#endif // DZBINARY_H
