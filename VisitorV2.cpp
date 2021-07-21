#include <numeric>

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

#include "VisitorV2.h"
#include "DzBinary.h"
#include "DzMemberAccess.h"
#include "DzConstant.h"
#include "DzReturn.h"
#include "DzClosure.h"
#include "DzParameter.h"
#include "DzFunctionCall.h"
#include "DzTerminator.h"
#include "CallContext.h"
#include "EntryPoint.h"
#include "DzFunction.h"
#include "DzTypeName.h"
#include "DzMember.h"
#include "DzConsumer.h"

VisitorV2::VisitorV2(FunctionAttribute attribute, DzValue *parent, DzValue *deepest)
	: m_attribute(attribute)
	, m_parent(parent)
	, m_deepest(deepest)
{
}

antlrcpp::Any VisitorV2::visitBlock(dzParser::BlockContext *ctx)
{
	return visit(ctx->ret());
}

antlrcpp::Any VisitorV2::visitBinary(dzParser::BinaryContext *ctx)
{
	auto binary = new DzBinary();

	VisitorV2 rightVisitor(m_attribute, binary, m_deepest);

	auto right = rightVisitor
		.visit(ctx->right)
		.as<Node>();

	VisitorV2 leftVisitor(m_attribute, right.root(), right.root());

	auto left = leftVisitor
		.visit(ctx->left)
		.as<Node>();

	binary->setLeft(left.value());
	binary->setRight(right.value());

	return Node(binary, left.root());
}

antlrcpp::Any VisitorV2::visitMember(dzParser::MemberContext *ctx)
{
	auto member = new DzMemberAccess(ctx->ID()->getText());

	return Node(member, m_deepest);
}

antlrcpp::Any VisitorV2::visitConstant(dzParser::ConstantContext *ctx)
{
	auto constant = new DzConstant(ctx->INT()->getText());

	return Node(constant, m_deepest);
}

CallContext fold(const CallContext &previous, dzParser::ExpressionContext *expression)
{
	auto attribute = previous.attribute();
	auto root = previous.root();
	auto call = previous.call();

	VisitorV2 visitor(attribute, root, root);

	auto node = visitor
		.visit(expression)
		.as<Node>();

	auto argument = node.value();
	auto function = node.root();

	call->addArgument(argument);

	return CallContext(attribute, function, call);
}

antlrcpp::Any VisitorV2::visitCall(dzParser::CallContext *ctx)
{
	static int i;

	auto ret = new DzReturn(m_deepest);
	auto closure = new DzClosure(i++, ret);
	auto call = new DzFunctionCall(ctx->ID()->getText(), closure);

	CallContext context(m_attribute, call, call);

	auto expression = ctx->expression();
	auto producer = std::accumulate(begin(expression), end(expression), context, &fold);

	return Node(closure->parameter(), producer.root());
}

FunctionAttribute getAttribute(dzParser::FunctionContext *ctx)
{
	if (ctx->attribute)
	{
		auto attribute = ctx->attribute->getText();

		if (attribute == "import")
		{
			return FunctionAttribute::Import;
		}

		if (attribute == "export")
		{
			return FunctionAttribute::Export;
		}
	}

	return FunctionAttribute::None;
}

antlrcpp::Any VisitorV2::visitFunction(dzParser::FunctionContext *ctx)
{
	auto name = ctx->name->getText();
	auto attribute = getAttribute(ctx);

	VisitorV2 visitor(attribute, m_parent, m_deepest);

	auto returnType = visitor
		.visit(ctx->typeName())
		.as<DzTypeName *>();

	std::vector<DzMember *> arguments;

	for (auto argument : ctx->argument())
	{
		auto argumentType = visitor
			.visit(argument)
			.as<DzMember *>();

		arguments.push_back(argumentType);
	}

	if (attribute == FunctionAttribute::Import)
	{
		return new DzFunction(name, arguments, FunctionAttribute::Import, returnType, nullptr);
	}

	if (attribute == FunctionAttribute::None)
	{
		auto context = new DzMember("parent"
			, new DzTypeName("context")
			);

		auto consumer = new DzMember("consumer"
			, new DzTypeName("consumer")
			);

		arguments.push_back(context);
		arguments.push_back(consumer);
	}

	auto block = ctx->block();

	if (block)
	{
		auto function = visitor
			.visit(block);

		auto value = function
			.as<Node>()
			.value();

		return new DzFunction(name, arguments, attribute, returnType, value);
	}

	return defaultResult();
}

antlrcpp::Any VisitorV2::visitProgram(dzParser::ProgramContext *ctx)
{
	std::map<std::string, DzFunction *> functions;
	std::vector<DzFunction *> roots;

	for (auto function : ctx->function())
	{
		auto q = visit(function)
			.as<DzFunction *>();

		switch (q->attribute())
		{
			case FunctionAttribute::None:
			{
				functions[q->name()] = q;

				break;
			}

			case FunctionAttribute::Import:
			{
				break;
			}

			case FunctionAttribute::Export:
			{
				roots.push_back(q);

				break;
			}
		}
	}

	llvm::LLVMContext context;
	llvm::Module module("dz", context);

	for (auto root : roots)
	{
		std::map<std::string, DzValue *> locals;

		EntryPoint entryPoint(nullptr, nullptr, &context, &module, nullptr, root->returnType(), functions, locals);

		root->build(entryPoint);
	}

	module.print(llvm::errs(), nullptr);

	std::string error;

	auto targetTriple = llvm::sys::getDefaultTargetTriple();
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

	if (!target)
	{
		llvm::errs() << error;

		return 1;
	}

	auto relocModel = llvm::Optional<llvm::Reloc::Model>();
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", llvm::TargetOptions(), relocModel);

	module.setDataLayout(targetMachine->createDataLayout());

	std::error_code EC;
	llvm::raw_fd_ostream dest("output.o", EC, llvm::sys::fs::OF_None);

	llvm::legacy::PassManager pm;

	targetMachine->addPassesToEmitFile(pm, dest, nullptr, llvm::CGFT_ObjectFile);

	pm.run(module);

	dest.flush();

	return defaultResult();
}

antlrcpp::Any VisitorV2::visitRet(dzParser::RetContext *ctx)
{
	if (m_attribute == FunctionAttribute::None)
	{
		auto call = new DzFunctionCall("consumer", nullptr);

		VisitorV2 visitor(m_attribute, call, call);

		auto node = visitor
			.visit(ctx->value)
			.as<Node>();

		auto value = node.value();
		auto root = node.root();

		call->addArgument(value);

		auto ret = new DzReturn(root);

		return Node(ret, root);
	}

	auto terminator = new DzTerminator();

	VisitorV2 visitor(m_attribute, terminator, terminator);

	auto node = visitor
		.visit(ctx->value)
		.as<Node>();

	auto value = node.value();
	auto root = node.root();

	terminator->setValue(value);

	auto ret = new DzReturn(root);

	return Node(ret, root);
}

antlrcpp::Any VisitorV2::visitTypeName(dzParser::TypeNameContext *ctx)
{
	return new DzTypeName(ctx->getText());
}

antlrcpp::Any VisitorV2::visitArgument(dzParser::ArgumentContext *ctx)
{
	auto name = ctx->ID()->getText();
	auto type = visit(ctx->typeName())
		.as<DzTypeName *>();

	return new DzMember(name, type);
}
