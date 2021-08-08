#ifndef DZEXPORTEDFUNCTION_H
#define DZEXPORTEDFUNCTION_H

#include "DzCallable.h"

class DzExportedFunction : public DzCallable
{
	public:
		DzExportedFunction(const std::string &name, DzValue *block);

		std::string name() const override;

		FunctionAttribute attribute() const override;

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		std::string m_name;

		DzValue *m_block;
};

#endif // DZEXPORTEDFUNCTION_H
