#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>

#include "EntryPointInfo.h"
#include "DzCallNg.h"
#include "DzFunctionNg.h"
#include "DzMemberNg.h"

DzCallNg::DzCallNg(const std::string &name, std::vector<DzValueNg *> parameters)
	: m_name(name)
	, m_parameters(parameters)
{
}

llvm::Value *DzCallNg::build(const EntryPointInfo &entryPoint) const
{
	auto &context = entryPoint.context();
	auto f = entryPoint.function();
	auto functions = entryPoint.functions();
	auto parent = entryPoint.block();

	auto block = llvm::BasicBlock::Create(*context, m_name, f, parent);

	auto iterator = functions.find(m_name);

	if (iterator == functions.end())
	{
		return nullptr;
	}

	auto epp = entryPoint
		.withBlock(block);

	auto function = iterator->second;

	auto locals = entryPoint.locals();

	auto arg = function->arguments();

	for (auto i = 0u; i < m_parameters.size() && i < arg.size(); i++)
	{
		auto name = arg[i]->name();

		locals[name] = m_parameters[i]->build(epp);
	}

	auto ep = epp
		.withLocals(locals);

	auto addressOfReturnValue = iterator->second->build(ep);

	llvm::IRBuilder<> builder(parent);

	return builder.CreateLoad(llvm::Type::getInt32Ty(*context), addressOfReturnValue);
}
