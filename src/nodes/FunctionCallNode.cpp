#include <iostream>
#include <numeric>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "nodes/FunctionCallNode.h"
#include "EntryPoint.h"
#include "Type.h"
#include "IndexIterator.h"
#include "AllIterator.h"
#include "IRBuilderEx.h"
#include "ZipIterator.h"
#include "ValueHelper.h"
#include "FunctionNotFoundException.h"
#include "FunctionHelper.h"

#include "nodes/FunctionNode.h"
#include "nodes/StackSegmentNode.h"

#include "values/ExpandedValue.h"
#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"
#include "values/ArrayValue.h"
#include "values/ExpandableValue.h"
#include "values/IteratorValue.h"
#include "values/FunctionValue.h"
#include "values/IteratorValueGenerator.h"
#include "values/LazyValue.h"

#include "types/IteratorType.h"

#include "exceptions/InvalidFunctionPointerTypeException.h"
#include "exceptions/AmbiguousFunctionException.h"
#include "exceptions/MissingTailCallException.h"

#include "iterators/ExtremitiesIterator.h"

FunctionCallNode::FunctionCallNode(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string> &names, const Node *evaluation)
	: m_ast(ast)
	, m_names(names)
	, m_evaluation(evaluation)
{
}

int FunctionCallNode::order(const EntryPoint &entryPoint) const
{
	auto functions = entryPoint.functions();

	for (auto &name : m_names)
	{
		auto iterator = functions.find(name);

		if (iterator != functions.end())
		{
			if (iterator->second->attribute() == FunctionAttribute::Iterator)
			{
				return 1;
			}
		}
	}

	return -1;
}

std::vector<DzResult> FunctionCallNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto [score, _1, _2] = FunctionHelper::tryCreateTailCall(entryPoint, values, begin(m_names), end(m_names));

	if (score == 0)
	{
		throw new MissingTailCallException(m_ast);
	}

	auto &context = entryPoint.context();

	std::vector<DzResult> result;

	for (auto &[resultEntryPoint, resultValues] : m_evaluation->build(entryPoint, values))
	{
		auto locals = resultEntryPoint.locals();
		auto parent = resultEntryPoint.function();
		auto block = resultEntryPoint.block();

		auto function = findFunction(resultEntryPoint, resultValues);

		if (!function)
		{
			throw new FunctionNotFoundException(m_ast, m_names[0], resultValues);
		}

		insertBlock(resultEntryPoint.block(), parent);

		auto functionBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, functionBlock);

		auto functionEntryPoint = resultEntryPoint
			.withBlock(functionBlock);

		if (function->attribute() == FunctionAttribute::Import)
		{
			return function->build(functionEntryPoint, resultValues);
		}

		auto functionResults = function->build(functionEntryPoint, resultValues);

		for (const auto &[lastEntryPoint, returnValue] : functionResults)
		{
			insertBlock(lastEntryPoint.block(), parent);

			auto consumerBlock = llvm::BasicBlock::Create(*context);

			linkBlocks(lastEntryPoint.block(), consumerBlock);

			auto consumerEntryPoint = functionEntryPoint
				.withDepth(lastEntryPoint.depth())
				.withBlock(consumerBlock);

			result.push_back({ consumerEntryPoint, returnValue });
		}
	}

	return result;
}

const CallableNode *FunctionCallNode::findFunction(const EntryPoint &entryPoint, Stack values) const
{
	auto functions = entryPoint.functions();
	auto locals = entryPoint.locals();

	for (auto &name : m_names)
	{
		auto local = locals.find(name);

		if (local != locals.end())
		{
			auto value = dynamic_cast<const FunctionValue *>(local->second);

			if (!value)
			{
				throw new InvalidFunctionPointerTypeException(m_ast, name);
			}

			return value->function();
		}

		std::map<int8_t, CallableNode *> candidates;

		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			auto score = function->signatureCompatibility(entryPoint, values);

			if (score < 0)
			{
				continue;
			}

			auto candidate = candidates.find(score);

			if (candidate != candidates.end())
			{
				std::vector<CallableNode *> functions = { candidate->second, function };

				throw new AmbiguousFunctionException(m_ast
					, functions
					, entryPoint
					);
			}

			candidates[score] = function;
		}

		if (candidates.size() > 0)
		{
			auto [_, function] = *candidates.begin();

			return function;
		}
	}

	return nullptr;
}
