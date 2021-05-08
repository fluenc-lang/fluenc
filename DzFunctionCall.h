#ifndef DZFUNCTIONCALL_H
#define DZFUNCTIONCALL_H

#include <vector>

#include <llvm/IR/Function.h>

#include "antlr4-runtime/dzParser.h"

#include "DzValue.h"

class DzClosure;
class DzReturn;
class DzParameter;

class DzFunctionCall : public DzValue
{
	public:
		DzFunctionCall(std::string name, DzValue *parent);

		void addArgument(DzValue *argument);

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		llvm::FunctionCallee getTarget(const EntryPoint &entryPoint) const;

		DzValue *m_parent;

		std::string m_name;
		std::vector<DzValue *> m_arguments;
};

#endif // DZFUNCTIONCALL_H
