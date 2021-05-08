#ifndef VISITORV1_H
#define VISITORV1_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <vector>

#include "UnknownTypeException.h"
#include "UndeclaredIdentifierException.h"

using namespace llvm;

class VisitorV1 : public dzBaseVisitor
{
	public:
		VisitorV1(LLVMContext &context, Module &module, BasicBlock *block, Function *function, std::map<std::string, Value *> locals);

//		antlrcpp::Any visitAssignment(dzParser::AssignmentContext *context) override;
		antlrcpp::Any visitFunction(dzParser::FunctionContext *context) override;
		antlrcpp::Any visitTypeName(dzParser::TypeNameContext *context) override;
//		antlrcpp::Any visitBlock(dzParser::BlockContext *context) override;
		antlrcpp::Any visitRet(dzParser::RetContext *context) override;
		antlrcpp::Any visitConstant(dzParser::ConstantContext *context) override;
		antlrcpp::Any visitMember(dzParser::MemberContext *context) override;
		antlrcpp::Any visitCall(dzParser::CallContext *context) override;
		antlrcpp::Any visitBinary(dzParser::BinaryContext *context) override;
		antlrcpp::Any visitConditional(dzParser::ConditionalContext *context) override;
		antlrcpp::Any visitStructure(dzParser::StructureContext *context) override;

	private:
//		VisitorV1 populateBlock(VisitorV1 visitor
//			, std::vector<dzParser::StatementContext *>::const_iterator iterator
//			, std::vector<dzParser::StatementContext *>::const_iterator end
//			);

		template<class T>
		T visitAs(tree::ParseTree *tree)
		{
			return tree->accept(this).as<T>();
		}

		LLVMContext &m_context;
		Module &m_module;
		BasicBlock *m_block;
		Function *m_function;
		std::map<std::string, Value *> m_locals;
};


#endif // VISITORV1_H
