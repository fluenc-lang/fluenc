#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>

#include <sstream>

#include "DzClosure.h"
#include "DzClosureAccessor.h"
#include "DzParameter.h"
#include "DzMemberAccess.h"
#include "EntryPoint.h"

DzClosure::DzClosure(int id, DzValue *parent)
	: m_id(id)
	, m_parent(parent)
{
}

DzValue *DzClosure::parameter()
{
	std::ostringstream pname;
	pname << "_";
	pname << m_id;

//	return new DzParameter(m_id);
	return new DzMemberAccess(pname.str());
}

class Kask : public DzValue
{

		// DzValue interface
	public:
		Kask(llvm::Value *value, llvm::Type *type)
			: m_value(value)
			, m_type(type)
		{
		}

		llvm::Value *build(const EntryPoint &entryPoint) const override
		{
			auto block = entryPoint.block();

			llvm::IRBuilder<> builder(block);

			return builder.CreateBitCast(m_value, m_type);
		}

	private:
		llvm::Value *m_value;
		llvm::Type *m_type;
};

llvm::Value *DzClosure::build(const EntryPoint &entryPoint) const
{
	std::ostringstream stream;
	stream << "closure_";
	stream << m_id;

	auto returnType = entryPoint.returnType();
	auto module = entryPoint.module();
	auto context = entryPoint.context();
	auto parent = entryPoint.parent();

	std::vector<llvm::Type *> argumentTypes =
	{
		llvm::Type::getInt32Ty(*context),
		llvm::Type::getInt32PtrTy(*context),
	};

//	std::ostringstream stream;
//	stream << m_parameter->toString(entryPoint);
//	stream << " => ";
//	stream << m_parent->toString(entryPoint);

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, stream.str(), module);
	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	std::map<std::string, DzValue *> locals;

	auto parentLocals = entryPoint.locals();

	for (auto &local : parentLocals)
	{
		locals[local.first] = new DzClosureAccessor(local.second);
	}

	std::ostringstream pname;
	pname << "_";
	pname << m_id;

	if (parent->closure())
	{
		locals["parent"] = new Kask(function->getArg(1), parent->closure()->getType());
	}

	locals[pname.str()] = new Kask(function->getArg(0), argumentTypes[0]);

	auto nested = entryPoint
		.withParent(&entryPoint)
		.withLocals(locals)
		.withBlock(block);

	m_parent->build(nested);

	verifyFunction(*function);

	return function;
}
