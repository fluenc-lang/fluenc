#include <llvm/IR/Verifier.h>

#include "Emitter.h"
#include "ValueHelper.h"
#include "IRBuilderEx.h"
#include "ITypeName.h"
#include "UndeclaredIdentifierException.h"
#include "IteratorStorage.h"
#include "FunctionNotFoundException.h"
#include "FunctionHelper.h"
#include "Indexed.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"
#include "ZipIterator.h"
#include "DzArgument.h"
#include "DzTupleArgument.h"
#include "InteropHelper.h"

#include "types/IOperatorSet.h"
#include "types/Int64Type.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/Int32Type.h"
#include "types/IPrototype.h"
#include "types/VoidType.h"

#include "values/ScalarValue.h"
#include "values/ExpandedValue.h"
#include "values/ExpandableValue.h"
#include "values/IndexedValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"
#include "values/StringValue.h"
#include "values/WithoutValue.h"
#include "values/FunctionValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"
#include "values/ForwardedValue.h"
#include "values/LazyValue.h"
#include "values/IIteratable.h"
#include "values/IteratorValueGenerator.h"
#include "values/ArrayValueGenerator.h"
#include "values/PlaceholderValue.h"
#include "values/ArrayValue.h"
#include "values/IteratorValue.h"
#include "values/StringIteratable.h"

#include "nodes/BinaryNode.h"
#include "nodes/ExportedFunctionNode.h"
#include "nodes/ArrayContinuationNode.h"
#include "nodes/ArrayElementNode.h"
#include "nodes/IntegralLiteralNode.h"
#include "nodes/FloatLiteralNode.h"
#include "nodes/BooleanLiteralNode.h"
#include "nodes/StringLiteralNode.h"
#include "nodes/CharacterLiteralNode.h"
#include "nodes/NothingNode.h"
#include "nodes/MemberAccessNode.h"
#include "nodes/ReferenceSinkNode.h"
#include "nodes/LazyEvaluationNode.h"
#include "nodes/FunctionCallNode.h"
#include "nodes/StackSegmentNode.h"
#include "nodes/FunctionCallProxyNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/InstantiationNode.h"
#include "nodes/ConditionalNode.h"
#include "nodes/BlockInstructionNode.h"
#include "nodes/EmptyArrayNode.h"
#include "nodes/IndexSinkNode.h"
#include "nodes/ArraySinkNode.h"
#include "nodes/ExpansionNode.h"
#include "nodes/LocalNode.h"
#include "nodes/ContinuationNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/TailFunctionCallNode.h"
#include "nodes/FunctionNode.h"
#include "nodes/ImportedFunctionNode.h"
#include "nodes/GlobalNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/ParentInjectorNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/IteratableNode.h"

#include "exceptions/InvalidFunctionPointerTypeException.h"
#include "exceptions/MissingTailCallException.h"
#include "exceptions/AmbiguousFunctionException.h"
#include "exceptions/MissingDefaultValueException.h"
#include "exceptions/MissingFieldException.h"
#include "exceptions/InvalidArgumentTypeException.h"

std::vector<DzResult> Emitter::visitBooleanBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = ValueHelper::getScalar(entryPoint, values);
	auto right = ValueHelper::getScalar(entryPoint, values);

	IRBuilderEx builder(entryPoint);

	auto valueFactory = [&]
	{
		if (node->m_op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->m_op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->m_op == "&&")
		{
			return builder.createLogicalAnd(left, right);
		}

		if (node->m_op == "||")
		{
			return builder.createLogicalOr(left, right);
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	values.push(value);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Emitter::visitFloatBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = ValueHelper::getScalar(entryPoint, values);
	auto right = ValueHelper::getScalar(entryPoint, values);

	IRBuilderEx builder(entryPoint);

	auto valueFactory = [&]
	{
		if (node->m_op == "+")
		{
			return builder.createFAdd(left, right);
		}

		if (node->m_op == "-")
		{
			return builder.createFSub(left, right);
		}

		if (node->m_op == "*")
		{
			return builder.createFMul(left, right);
		}

		if (node->m_op == "/")
		{
			return builder.createFDiv(left, right);
		}

		if (node->m_op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULT, left, right);
		}

		if (node->m_op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_ULE, left, right);
		}

		if (node->m_op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGT, left, right);
		}

		if (node->m_op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UGE, left, right);
		}

		if (node->m_op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UEQ, left, right);
		}

		if (node->m_op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::FCMP_UNE, left, right);
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	values.push(value);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Emitter::visitIntegerBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = ValueHelper::getScalar(entryPoint, values);
	auto right = ValueHelper::getScalar(entryPoint, values);

	IRBuilderEx builder(entryPoint);

	auto valueFactory = [&]
	{
		if (node->m_op == "+")
		{
			return builder.createAdd(left, right);
		}

		if (node->m_op == "-")
		{
			return builder.createSub(left, right);
		}

		if (node->m_op == "*")
		{
			return builder.createMul(left, right);
		}

		if (node->m_op == "/")
		{
			return builder.createSDiv(left, right);
		}

		if (node->m_op == "<")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
		}

		if (node->m_op == "<=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
		}

		if (node->m_op == ">")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
		}

		if (node->m_op == ">=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
		}

		if (node->m_op == "==")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
		}

		if (node->m_op == "!=")
		{
			return builder.createCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);
		}

		if (node->m_op == "%")
		{
			return builder.createSRem(left, right);
		}

		if (node->m_op == "|")
		{
			return builder.createOr(left, right);
		}

		if (node->m_op == "&")
		{
			return builder.createAnd(left, right);
		}

		if (node->m_op == "^")
		{
			return builder.createXor(left, right);
		}

		throw new std::exception();
	};

	auto value = valueFactory();

	values.push(value);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Emitter::visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto left = values.peek();

	auto leftType = left->type();
	auto operators = leftType->operators();

	auto binary = operators->forBinary(node);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : binary->accept(*this, entryPoint, values))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto module = entryPoint.module();
	auto context = entryPoint.context();

	auto returnType = node->m_returnType->resolve(entryPoint);
	auto storageType = returnType->storageType(*context);

	std::vector<llvm::Type *> argumentTypes;

	auto functionType = llvm::FunctionType::get(storageType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, node->m_name, module);

	auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
	auto block = llvm::BasicBlock::Create(*context);

	linkBlocks(alloc, block);

	auto ep = entryPoint
		.withFunction(function)
		.withBlock(block)
		.withAlloc(alloc);

	auto result = node->m_block->accept(*this, ep, values);

	verifyFunction(*function, &llvm::errs());

	return result;
}

std::vector<DzResult> Emitter::visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto context = entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto indexConstant = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 1)
		);

	IRBuilderEx builder(entryPoint);

	auto load = builder.createLoad(node->m_index, "index");
	auto add = builder.createAdd(load, indexConstant);

	builder.createStore(add, node->m_index);

	auto value = new ExpandedValue(true
	, node->m_iteratorType
		, entryPoint
		, node->m_node
		, node
		, std::vector<const ExpandedValue *>()
		);

	return {{ entryPoint, value }};
}

std::vector<DzResult> Emitter::visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack values) const
{
	Stack valuesIfTrue;

	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	auto dataLayout = module->getDataLayout();

	auto index = values.require<ReferenceValue>(nullptr);
	auto value = values.require<IndexedValue>(nullptr);

	if (node->m_next)
	{
		auto valuesIfFalse = values;

		insertBlock(block, function);

		auto indexType = index->type();
		auto storageType = indexType->storageType(*context);

		auto ifTrue = llvm::BasicBlock::Create(*context);
		auto ifFalse = llvm::BasicBlock::Create(*context);

		IRBuilderEx builder(entryPoint);

		auto indexLoad = builder.createLoad(index, "index");

		auto indexConstant = new ScalarValue(indexType
			, llvm::ConstantInt::get(storageType, value->index())
			);

		auto comparison =  builder.createCmp(llvm::CmpInst::Predicate::ICMP_EQ, indexLoad, indexConstant);

		builder.createCondBr(comparison , ifTrue, ifFalse);

		auto epIfFalse = entryPoint
			.withIndex(value->index())
			.withBlock(ifFalse);

		auto epIfTrue = entryPoint
			.withIndex(value->index())
			.withBlock(ifTrue);

		valuesIfFalse.push(index);

		auto continuation = new ArrayContinuationNode(index, node->m_node, IteratorType::instance());
		auto expandable = new ExpandableValue(true, node->m_arrayType, entryPoint, continuation);

		auto tuple = new TupleValue({ expandable, value->subject() });

		valuesIfTrue.push(tuple);

		auto resultsIfFalse = node->m_next->accept(*this, epIfFalse, valuesIfFalse);

		std::vector<DzResult> result = {{ epIfTrue, valuesIfTrue }};

		result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

		return result;
	}

	auto ep = entryPoint
		.withIndex(value->index());

	values.push(value->subject());

	return {{ ep, values }};
}

std::vector<DzResult> Emitter::visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = node->m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto valueProvider = [&]
	{
		auto hex = node->m_value.find("0x");

		if (hex != std::string::npos)
		{
			return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->m_value.substr(hex + 2), 16);
		}

		return llvm::ConstantInt::get((llvm::IntegerType *)storageType, node->m_value, 10);
	};

	auto value = new ScalarValue(type
		, valueProvider()
		);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = node->m_type->resolve(entryPoint);
	auto storageType = type->storageType(*context);

	auto value = new ScalarValue(type
		, llvm::ConstantFP::get(storageType, node->m_value)
		);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto valueProvider = [&]
	{
		auto type = BooleanType::instance();
		auto storageType = type->storageType(*context);

		if (node->m_value == "true")
		{
			return new ScalarValue { type, llvm::ConstantInt::get(storageType, 1) };
		}

		if (node->m_value == "false")
		{
			return new ScalarValue { type, llvm::ConstantInt::get(storageType, 0) };
		}

		throw new std::exception(); // TODO
	};

	auto value = valueProvider();

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	IRBuilderEx builder(entryPoint);

	auto stringType = StringType::get(node->m_value.size());

	auto string = new ScalarValue(stringType
		, builder.createGlobalStringPtr(node->m_value, "string")
		);

	auto alloc = entryPoint.alloc(stringType);

	builder.createStore(string, alloc);

	values.push(new StringValue(node, alloc, node->id(), node->m_value.size()));

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto type = Int32Type::instance();
	auto storageType = type->storageType(*context);

	auto valueProvider = [&]
	{
		static std::unordered_map<std::string, std::string> sequences =
		{
			{ "\\n", "\n" },
			{ "\\r", "\r" },
			{ "\\t", "\t" },
			{ "\\0", "\0" },
		};

		auto iterator = sequences.find(node->m_value);

		if (iterator != sequences.end())
		{
			return llvm::ConstantInt::get((llvm::IntegerType *)storageType
				, *iterator->second.begin()
				);
		}

		return llvm::ConstantInt::get((llvm::IntegerType *)storageType
			, *node->m_value.begin()
			);
	};

	auto value = new ScalarValue(type
		, valueProvider()
		);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto &locals = entryPoint.locals();
	auto &functions = entryPoint.functions();
	auto &globals = entryPoint.globals();

	for (auto &name : node->m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			if (auto value = dynamic_cast<const ReferenceValue *>(localsIterator->second))
			{
				IRBuilderEx builder(entryPoint);

				auto load = builder.createLoad(value, name);

				values.push(load);
			}
			else if (localsIterator->second)
			{
				auto forwarded = localsIterator->second->forward(node->id());

				values.push(forwarded);
			}

			return node->m_consumer->accept(*this, entryPoint, values);
		}

		auto functionsIterator = functions.find(name);

		if (functionsIterator != functions.end())
		{
			auto value = new FunctionValue(functionsIterator->second, entryPoint);

			values.push(value);

			return node->m_consumer->accept(*this, entryPoint, values);
		}

		auto globalsIterator = globals.find(name);

		if (globalsIterator != globals.end())
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : globalsIterator->second->accept(*this, entryPoint, values))
			{
				for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}
	}

	throw new UndeclaredIdentifierException(node->m_ast, node->m_names[0]);
}

std::vector<DzResult> Emitter::visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto makeReference = [&](const BaseValue *value, auto &recurse) -> const BaseValue *
	{
		IRBuilderEx builder(entryPoint);

		if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
		{
			auto argumentType = typedValue->type();

			auto alloc = entryPoint.alloc(argumentType);

			builder.createStore(typedValue, alloc);

			return alloc;
		}

		if (auto referenceValue = dynamic_cast<const ReferenceValue *>(value))
		{
			auto argumentType = referenceValue->type();

			auto load = builder.createLoad(referenceValue);

			auto alloc = entryPoint.alloc(argumentType);

			builder.createStore(load, alloc);

			return alloc;
		}

		if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
		{
			auto tupleValues = tupleValue->values();

			std::vector<const BaseValue *> values;

			std::transform(tupleValues.begin(), tupleValues.end(), std::back_inserter(values), [&](auto value)
			{
				return recurse(value, recurse);
			});

			return new TupleValue(values);
		}

		if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
		{
			auto fields = userTypeValue->fields();

			std::vector<const NamedValue *> values;

			std::transform(begin(fields), end(fields), std::back_inserter(values), [&](auto field)
			{
				return new NamedValue { field->name(), recurse(field->value(), recurse) };
			});

			return new UserTypeValue { userTypeValue->prototype(), values };
		}

		return value;
	};

	auto value = makeReference(values.pop(), makeReference);

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto digestDepth = [this](const EntryPoint &entryPoint, Stack values, auto &recurse) -> std::vector<DzResult>
	{
		for (auto i = 0u; i < values.size(); i++)
		{
			auto value = values.pop();

			if (auto forwarded = dynamic_cast<const ForwardedValue *>(value))
			{
				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : recurse(entryPoint, values, recurse))
				{
					std::vector<const BaseValue *> forwardedValues;

					for (auto resultValue : resultValues)
					{
						forwardedValues.push_back(resultValue);
					}

					auto subject = forwarded->subject();

					forwardedValues.push_back(subject);

					results.push_back({ resultEntryPoint, forwardedValues });
				}

				return results;
			}

			if (auto lazy = dynamic_cast<const LazyValue *>(value))
			{
				auto iteratable = lazy->generate(entryPoint);

				std::vector<DzResult> results;

				for (auto &[resultEntryPoint, resultValues] : iteratable->accept(*this, entryPoint, Stack()))
				{
					auto forwardedValues = values;

					for (auto &resultValue : resultValues)
					{
						forwardedValues.push(resultValue);
					}

					auto forwardedEntryPoint = resultEntryPoint
						.withIteratorStorage(entryPoint.iteratorStorage());

					for (auto &result : recurse(forwardedEntryPoint, forwardedValues, recurse))
					{
						results.push_back(result);
					}
				}

				return results;
			}

			if (auto string = dynamic_cast<const StringValue *>(value))
			{
				auto iterator =	string->iterator();

				auto forwardedValues = values;

				forwardedValues.push(iterator);

				return recurse(entryPoint, forwardedValues, recurse);
			}

			if (auto tuple = dynamic_cast<const TupleValue *>(value))
			{
				for (auto &element : tuple->values())
				{
					if (auto expanded = dynamic_cast<const ExpandedValue *>(element))
					{
						auto node = expanded->node();

						std::vector<DzResult> results;

						for (auto &[resultEntryPoint, resultValues] : node->accept(*this, entryPoint, values))
						{
							auto forwardedValues = values;

							for (auto &resultValue : resultValues)
							{
								forwardedValues.push(resultValue);
							}

							auto forwardedEntryPoint = resultEntryPoint
								.withIteratorStorage(entryPoint.iteratorStorage());

							for (auto &result : recurse(forwardedEntryPoint, forwardedValues, recurse))
							{
								results.push_back(result);
							}
						}

						return results;
					}
				}

				std::vector<DzResult> results;

				auto digestedResults = recurse(entryPoint
					, tuple->values()
					, recurse
					);

				for (auto &[digestedEntryPoint, digestedValues] : digestedResults)
				{
					auto digestedTuple = new TupleValue({ digestedValues.begin(), digestedValues.end() });

					for (auto &[resultEntryPoint, resultValues] : recurse(digestedEntryPoint, values, recurse))
					{
						std::vector<const BaseValue *> forwardedValues;

						for (auto resultValue : resultValues)
						{
							forwardedValues.push_back(resultValue);
						}

						forwardedValues.push_back(digestedTuple);

						results.push_back({ resultEntryPoint, forwardedValues });
					}
				}

				return results;
			}

			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : recurse(entryPoint, values, recurse))
			{
				std::vector<const BaseValue *> forwardedValues;

				for (auto resultValue : resultValues)
				{
					forwardedValues.push_back(resultValue);
				}

				forwardedValues.push_back(value);

				results.push_back({ resultEntryPoint, forwardedValues });
			}

			return results;
		}

		return {{ entryPoint, values }};
	};

	auto tryForkEntryPoint = [&]
	{
		if (entryPoint.iteratorStorage())
		{
			return entryPoint;
		}

		return entryPoint
			.withIteratorStorage(new IteratorStorage());
	};

	auto forkedEntryPoint = tryForkEntryPoint();

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(forkedEntryPoint, values, digestDepth))
	{
		auto consumerEntryPoint = resultEntryPoint
			.withIteratorStorage(entryPoint.iteratorStorage());

		for (auto &result : node->m_consumer->accept(*this, consumerEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto findFunction = [&](const std::vector<const Type *> &types) -> const CallableNode *
	{
		auto &functions = entryPoint.functions();
		auto &locals = entryPoint.locals();

		for (auto &name : node->m_names)
		{
			auto local = locals.find(name);

			if (local != locals.end())
			{
				auto value = dynamic_cast<const FunctionValue *>(local->second);

				if (!value)
				{
					throw new InvalidFunctionPointerTypeException(node->m_ast, name);
				}

				return value->function();
			}

			std::map<int8_t, CallableNode *> candidates;

			for (auto [i, end] = functions.equal_range(name); i != end; i++)
			{
				auto function = i->second;

				auto score = function->signatureCompatibility(entryPoint, types);

				if (score < 0)
				{
					continue;
				}

				auto candidate = candidates.find(score);

				if (candidate != candidates.end())
				{
					std::vector<CallableNode *> functions = { candidate->second, function };

					throw new AmbiguousFunctionException(node->m_ast
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
	};

	auto [score, _1, _2] = FunctionHelper::tryCreateTailCall(entryPoint, values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		throw new MissingTailCallException(node->m_ast);
	}

	auto context = entryPoint.context();

	std::vector<DzResult> result;

	for (auto &[resultEntryPoint, resultValues] : node->m_evaluation->accept(*this, entryPoint, values))
	{
		auto parent = resultEntryPoint.function();
		auto block = resultEntryPoint.block();

		std::vector<const Type *> types;

		std::transform(resultValues.rbegin(), resultValues.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

		auto function = findFunction(types);

		if (!function)
		{
			throw new FunctionNotFoundException(node->m_ast, node->m_names[0], types);
		}

		insertBlock(block, parent);

		auto functionBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, functionBlock);

		auto functionEntryPoint = resultEntryPoint
			.withBlock(functionBlock);

		if (function->attribute() == FunctionAttribute::Import)
		{
			return function->accept(*this, functionEntryPoint, resultValues);
		}

		auto functionResults = function->accept(*this, functionEntryPoint, resultValues);

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

std::vector<DzResult> Emitter::visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	std::vector<Indexed<Node *>> orderedValues;

	std::transform(begin(node->m_values), end(node->m_values), index_iterator(), std::back_inserter(orderedValues), [](auto value, auto index) -> Indexed<Node *>
	{
		return { index, value };
	});

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first.value->order(entryPoint) < second.value->order(entryPoint);
	});

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument.value->accept(*this, accumulatorEntryPoint, Stack());

			for (auto &[resultEntryPoint, resultValues] : result)
			{
				auto scopedReturnValues = accumulatorValues;

				for (auto resultValue : resultValues)
				{
					auto returnValue = new IndexedValue{ argument.index, resultValue };

					scopedReturnValues.push(returnValue);
				}

				results.push_back({ resultEntryPoint, scopedReturnValues });
			}
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		std::multimap<int, const BaseValue *, std::greater<int>> indexedValues;

		for (auto &value : subjectValues)
		{
			auto indexed = static_cast<const IndexedValue *>(value);

			indexedValues.insert({ indexed->index(), indexed->subject() });
		}

		Stack pointersToValues;

		for (auto [_, value] : indexedValues)
		{
			pointersToValues.push(value);
		}

		auto callResults = node->m_call->accept(*this, subjectEntryPoint, pointersToValues);

		for (auto &[callEntryPoint, callValues] : callResults)
		{
			auto forwardedValues = values;

			for (auto &value : callValues)
			{
				forwardedValues.push(value);
			}

			auto consumerResults = node->m_consumer->accept(*this, callEntryPoint, forwardedValues);

			for (auto &consumerResult : consumerResults)
			{
				result.push_back(consumerResult);
			}
		}
	}

	return result;
}

std::vector<DzResult> Emitter::visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto &functions = entryPoint.functions();

	for (auto &name : node->m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto generator = new IteratorValueGenerator(new IteratorType(), node->m_regularCall, entryPoint);
				auto lazy = new LazyValue(generator, entryPoint);

				auto forwardedValues = values;

				forwardedValues.push(lazy);

				std::vector<DzResult> results;

				for (auto &result : node->m_consumer->accept(*this, entryPoint, forwardedValues))
				{
					results.push_back(result);
				}

				return results;
			}
		}
	}

	std::vector<DzResult> results;

	auto junction = new JunctionNode(node->m_regularCall);

	for (auto &[resultEntryPoint, resultValues] : junction->accept(*this, entryPoint, values))
	{
		for (auto &result : node->m_consumer->accept(*this, resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
		const BaseValue *value;
	};

	auto join = [this](const std::vector<SingleResult> &range, const EntryPoint &entryPoint) -> const BaseValue *
	{
		auto function = entryPoint.function();

		auto [_, first] = *range.begin();

		if (auto templateValue = dynamic_cast<const ScalarValue *>(first))
		{
			auto type = templateValue->type();

			auto alloc = entryPoint.alloc(type);

			for (auto &[resultEntryPoint, value] : range)
			{
				auto transferEntryPoint = ValueHelper::transferValue(resultEntryPoint, value, alloc, *this);

				insertBlock(transferEntryPoint.block(), function);
				linkBlocks(transferEntryPoint.block(), entryPoint.block());
			}

			IRBuilderEx junctionBuilder(entryPoint);

			return junctionBuilder.createLoad(alloc, "junctionLoad");
		}

		auto alloc = first->clone(entryPoint);

		for (auto &[resultEntryPoint, value] : range)
		{
			auto transferEntryPoint = ValueHelper::transferValue(resultEntryPoint, value, alloc, *this);

			insertBlock(transferEntryPoint.block(), function);
			linkBlocks(transferEntryPoint.block(), entryPoint.block());
		}

		return alloc;
	};

	auto tryJoin = [&](const std::vector<SingleResult> &values, const EntryPoint &entryPoint) -> SingleResult
	{
		if (values.size() > 1)
		{
			auto context = entryPoint.context();

			auto junctionBlock = llvm::BasicBlock::Create(*context, "junction");

			auto junctionEntryPoint = entryPoint
				.withBlock(junctionBlock);

			auto joined = join(values, junctionEntryPoint);

			return { junctionEntryPoint, joined };
		}

		return values[0];
	};

	auto inputResults = node->m_subject->accept(*this, entryPoint, values);

	std::vector<DzResult> outputResults;

	std::multimap<const Type *, SingleResult> groupedResults;

	for (auto &result : inputResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			outputResults.push_back(result);

			continue;
		}

		auto value = resultValues.pop();

		groupedResults.insert({ value->type(), { resultEntryPoint, value } });
	}

	for (auto it = begin(groupedResults)
		 ; it != end(groupedResults)
		 ; it = upper_bound(it, end(groupedResults), *it, &compareKey<const Type *, SingleResult>)
		 )
	{
		auto [type, _] = *it;

		auto range = groupedResults.equal_range(type);

		std::vector<SingleResult> inputValues;

		std::transform(range.first, range.second, std::back_inserter(inputValues), [](auto pair)
		{
			return pair.second;
		});

		auto [joinedEntryPoint, joinedValue] = tryJoin(inputValues, entryPoint);

		outputResults.push_back({ joinedEntryPoint, joinedValue });
	}

	return outputResults;
}

std::vector<DzResult> Emitter::visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(node->m_fields), end(node->m_fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = node->m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_inserter(namedValues), [&](auto field) -> const NamedValue *
	{
		auto valueByName = valuesByName.find(field.name());

		if (valueByName != valuesByName.end())
		{
			auto value = valueByName->second;

			valuesByName.erase(valueByName);

			if (auto reference = dynamic_cast<const ReferenceValue *>(field.defaultValue()))
			{
				if (reference->type() != value->type())
				{
					return new NamedValue { field.name(), value };
				}

				auto type = reference->type();
				auto storageType = type->storageType(*context);

				auto align = dataLayout.getABITypeAlign(storageType);

				auto typedValue = ValueHelper::getScalar(entryPoint, value);

				auto store = new llvm::StoreInst(*typedValue, *reference, false, align, block);

				UNUSED(store);

				return new NamedValue { field.name(), field.defaultValue() };
			}

			return new NamedValue { field.name(), value };
		}

		if (!field.defaultValue())
		{
			throw new MissingDefaultValueException(node->m_ast, field.name());
		}

		return new NamedValue { field.name(), field.defaultValue() };
	});

	for (auto &[name, _] : valuesByName)
	{
		throw new MissingFieldException(node->m_ast, prototype->name(), name);
	}

	IRBuilderEx builder(entryPoint);

	std::vector<const NamedValue *> finalValues;

	std::transform(begin(namedValues), end(namedValues), std::back_inserter(finalValues), [&](auto field) -> const NamedValue *
	{
		auto value = field->value();

		if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
		{
			auto type = typedValue->type();

			auto alloc = entryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

			return new NamedValue { field->name(), alloc };
		}

		return field;
	});

	auto userTypeValue = new UserTypeValue(prototype, finalValues);

	values.push(userTypeValue);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
		const ScalarValue *value;
	};

	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto function = entryPoint.function();
	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	block->setName("condition");

	insertBlock(block, function);

	auto ifTrue = llvm::BasicBlock::Create(*context);
	auto ifFalse = llvm::BasicBlock::Create(*context);

	IRBuilderEx builder(entryPoint);

	auto conditional = ValueHelper::getScalar(entryPoint, values);

	builder.createCondBr(conditional, ifTrue, ifFalse);

	auto epIfFalse = entryPoint
		.withName("ifFalse")
		.withBlock(ifFalse);

	auto epIfTrue = entryPoint
		.withName("ifTrue")
		.withBlock(ifTrue);

	auto resultsIfTrue = node->m_ifTrue->accept(*this, epIfTrue, values);
	auto resultsIfFalse = node->m_ifFalse->accept(*this, epIfFalse, values);

	std::vector<DzResult> immediateResults;

	immediateResults.insert(end(immediateResults), begin(resultsIfTrue), end(resultsIfTrue));
	immediateResults.insert(end(immediateResults), begin(resultsIfFalse), end(resultsIfFalse));

	std::multimap<const Type *, SingleResult> groupedResults;

	for (auto &result : immediateResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			continue;
		}

		auto value = resultValues.request<ScalarValue>();

		if (value)
		{
			groupedResults.insert({ value->type(), { resultEntryPoint, value } });
		}
		else
		{
			return immediateResults;
		}
	}

	std::vector<DzResult> mergedResults;

	for(auto it = begin(groupedResults)
		; it != end(groupedResults)
		; it = upper_bound(it, end(groupedResults), *it, &compareKey<const Type *, SingleResult>)
		)
	{
		auto [type, _] = *it;

		auto range = groupedResults.equal_range(type);

		auto alloc = entryPoint.alloc(type);

		auto mergeBlock = llvm::BasicBlock::Create(*context, "merge", function);

		for (auto i = range.first; i != range.second; i++)
		{
			auto [resultEntryPoint, value] = i->second;

			auto resultBlock = resultEntryPoint.block();

			IRBuilderEx resultBuilder(resultEntryPoint);

			resultBuilder.createStore(value, alloc);

			linkBlocks(resultBlock, mergeBlock);
		}

		auto mergeEntryPoint = entryPoint
			.withBlock(mergeBlock);

		IRBuilderEx mergeBuilder(mergeEntryPoint);

		auto mergeLoad = mergeBuilder.createLoad(alloc, "mergeLoad");

		auto mergeValues = values;

		mergeValues.push(mergeLoad);

		mergedResults.push_back({ mergeEntryPoint, mergeValues });
	}

	return mergedResults;
}

std::vector<DzResult> Emitter::visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack values) const
{
	values.push(WithoutValue::instance());

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto value = values.pop();

	auto indexed = new IndexedValue(node->m_index, value);

	values.push(indexed);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto arrayContents = node->m_firstValue->accept(*this, entryPoint, Stack());

	auto generator = new ArrayValueGenerator(arrayContents, node->id(), node->m_size);
	auto lazy = new LazyValue(generator, entryPoint);

	values.push(lazy);

	std::vector<DzResult> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : node->m_consumer->accept(*this, arrayEntryPoint, values))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto expandable = values.require<ExpandableValue>(node->m_ast);

	auto continuation = expandable->chain();
	auto provider = expandable->provider();
	auto continuationEntryPoint = provider->withBlock(block);

	auto result = continuation->accept(*this, continuationEntryPoint, Stack());

	for (auto &[targetEntryPoint, continuationValues] : result)
	{
		auto value = continuationValues
			.require<ExpandedValue>(nullptr);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		values.push(tuple);

		auto consumerEntryPoint = entryPoint
			.withBlock(targetEntryPoint.block());

		return node->m_consumer->accept(*this, consumerEntryPoint, values);
	}

	throw new std::exception();
}

std::vector<DzResult> Emitter::visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();

	auto value = values.pop();

	if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
	{
		auto fields = userValue->fields();

		std::transform(begin(fields), end(fields), std::inserter(locals, begin(locals)), [=](auto field) -> std::pair<std::string, const BaseValue *>
		{
			std::stringstream ss;
			ss << node->m_name;
			ss << ".";
			ss << field->name();

			return { ss.str(), field->value() };
		});
	}

	locals[node->m_name] = value;

	auto ep = entryPoint
		.withLocals(locals);

	return node->m_consumer->accept(*this, ep, values);
}

std::vector<DzResult> Emitter::visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack values) const
{
	insertBlock(entryPoint.block(), entryPoint.function());

	auto numberOfArguments = values.size();

	auto inputValues = values;
	auto tailCallValues = entryPoint.values();

	auto tailCallCandidate = std::accumulate(index_iterator(0u), index_iterator(numberOfArguments), entryPoint, [&](auto target, size_t)
	{
		return ValueHelper::transferValue(target
			, inputValues.pop()
			, tailCallValues.pop()
			, *this
			);
	});

	auto next = ValueHelper::extractValues<ExpandedValue>(values);

	auto isArray = accumulate(begin(next), end(next), next.size() > 0, [](auto accumulated, auto value)
	{
		return accumulated && value->isArray();
	});

	auto value = new ExpandedValue(isArray
		, node->m_iteratorType
		, tailCallCandidate
		, node->m_node
		, node
		, next
		);

	return {{ tailCallCandidate, value }};
}

std::vector<DzResult> Emitter::visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto operand = values.pop();

	auto resolveOp = [&]
	{
		if (node->m_op == "@")
		{
			return new ForwardedValue(operand);
		}

		throw new std::exception(); // TODO
	};

	auto value = resolveOp();

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(entryPoint, values, begin(node->m_names), end(node->m_names));

	if (score == 0)
	{
		auto zipped = zip(values, targetValues);

		auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto result)
		{
			auto [value, storage] = result;

			return ValueHelper::transferValue(accumulatedEntryPoint, value, storage, *this);
		});

		linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw new std::exception(); // TODO
	}

	auto junction = new JunctionNode(node->m_regularCall);

	return junction->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	auto handleArgument = [&](DzBaseArgument *argument, const BaseValue *value, auto &recurse)
	{
		if (auto standardArgument = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();

			std::vector<Argument> result
			{
				{ name, value }
			};

			if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
			{
				auto fields = userValue->fields();

				std::transform(begin(fields), end(fields), std::back_inserter(result), [=](auto field) -> Argument
				{
					std::stringstream ss;
					ss << name;
					ss << ".";
					ss << field->name();

					return { ss.str(), field->value() };
				});
			}

			return result;
		}

		if (auto tupleArgument = dynamic_cast<DzTupleArgument *>(argument))
		{
			auto tupleValue = dynamic_cast<const TupleValue *>(value);

			auto tupleValues = tupleValue->values();
			auto arguments = tupleArgument->arguments();

			std::vector<Argument> results;

			for (auto argument : arguments)
			{
				for (auto &result : recurse(argument, tupleValues.pop(), recurse))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		throw new std::exception();
	};

	auto pep = entryPoint
		.withValues(values);

	std::map<std::string, const BaseValue *> locals;

	for (const auto &argument : node->m_arguments)
	{
		for (auto &[name, value] : handleArgument(argument, values.pop(), handleArgument))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(node->m_name)
		.markEntry()
		.withLocals(locals)
		.withIteratorStorage(nullptr);

	return node->m_block->accept(*this, ep, values);
}

std::vector<DzResult> Emitter::visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);

	auto context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();

	insertBlock(previous, function);

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	linkBlocks(previous, block);

	auto ep = entryPoint
		.withBlock(block);

	auto returnValue = ValueHelper::getScalar(ep, values);

	IRBuilderEx builder(ep);
	builder.createRet(*returnValue);

	return {{ ep, values }};
}

std::vector<DzResult> Emitter::visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto module = entryPoint.module();
	auto context = entryPoint.context();

	auto returnType = node->m_returnType->resolve(entryPoint);

	std::vector<llvm::Type *> argumentTypes;
	std::vector<llvm::Value *> argumentValues;

	IRBuilderEx builder(entryPoint);

	for (const auto &argument : node->m_arguments)
	{
		if (auto standardArgument  = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();
			auto type = standardArgument->type(entryPoint);

			auto storageType = type->storageType(*context);

			argumentTypes.push_back(storageType);

			auto value = values.pop();

			if (auto addressOfArgument = dynamic_cast<const ReferenceValue *>(value))
			{
				auto load = builder.createLoad(addressOfArgument, name);

				argumentValues.push_back(*load);
			}
			else if (auto stringValue = dynamic_cast<const StringValue *>(value))
			{
				auto load = builder.createLoad(stringValue->reference());

				argumentValues.push_back(*load);
			}
			else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
			{
				auto cast = InteropHelper::createWriteProxy(userTypeValue, entryPoint);

				argumentValues.push_back(cast);
			}
		}
		else
		{
			throw new InvalidArgumentTypeException(node->m_ast);
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*context), argumentTypes, false);

	auto function = module->getOrInsertFunction(node->m_name, functionType);

	auto call = builder.createCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		auto returnValue = InteropHelper::createReadProxy(call, returnType, entryPoint, node->m_ast);

		values.push(returnValue);
	}

	return {{ entryPoint, values }};
}

std::vector<DzResult> Emitter::visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_value->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack values) const
{
	auto fetchValue = [&]() -> const BaseValue *
	{
		auto value = values.pop();

		if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
		{
			IRBuilderEx builder(entryPoint);

			auto type = value->type();

			auto alloc = entryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

			return alloc;
		}

		return value;
	};

	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto value = fetchValue();

	if (node->m_chained)
	{
		auto expandable = new ExpandableValue(false, node->m_iteratorType, entryPoint, node->m_chained);
		auto tuple = new TupleValue({ expandable, value });

		values.push(tuple);

		return node->m_consumer->accept(*this, entryPoint, values);
	}

	values.push(value);

	return node->m_consumer->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	return node->m_subject->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return node->m_consumer->accept(*this, entryPoint, Stack());
}

std::vector<DzResult> Emitter::visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(node);

	return {{ entryPoint, values }};
}

std::vector<DzResult> Emitter::visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return node->m_iteratable->accept(*this, entryPoint, values);
}

std::vector<DzResult> Emitter::visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto context = entryPoint.context();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	insertBlock(block, function);

	std::vector<DzResult> results;

	for (auto [_, elementValues] : node->m_values)
	{
		elementValues.push(node->m_index);

		auto arrayBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, arrayBlock);

		auto iteratorEntryPoint = entryPoint
			.withBlock(arrayBlock)
			.withName("__array")
			.markEntry()
			;

		for (auto &result : node->m_iterator->accept(*this, iteratorEntryPoint, elementValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

std::vector<DzResult> Emitter::visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto locals = entryPoint.locals();

	for (auto &[key, value] : node->m_entryPoint->locals())
	{
		locals[key] = value;
	}

	auto ep = (*node->m_entryPoint)
		.withBlock(entryPoint.block())
		.withLocals(locals)
		.withIteratorStorage(entryPoint.iteratorStorage());

	return node->m_subject->accept(*this, ep, Stack());
}

std::vector<DzResult> Emitter::visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto context = entryPoint.context();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	insertBlock(block, function);

	auto iteratorBlock = llvm::BasicBlock::Create(*context, "iterator", function);

	linkBlocks(block, iteratorBlock);

	auto iteratorEntryPoint = entryPoint
		.withBlock(iteratorBlock)
		.withName("__iterator")
		.markEntry();

	IRBuilderEx builder(iteratorEntryPoint);

	auto indexType = node->m_index->type();
	auto storageType = indexType->storageType(*context);

	auto ifTrue = llvm::BasicBlock::Create(*context);
	auto ifFalse = llvm::BasicBlock::Create(*context);

	auto length = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, node->m_length - 1)
		);

	auto index = builder.createLoad(node->m_index);

	auto characterType = llvm::Type::getInt8Ty(*context);
	auto stringType = llvm::Type::getInt8PtrTy(*context);

	auto cast = new llvm::BitCastInst(node->m_address, stringType, "stringCast", iteratorBlock);

	auto gep = new ReferenceValue(Int32Type::instance()
		, llvm::GetElementPtrInst::CreateInBounds(characterType, cast, { *index }, "stringAccess", iteratorBlock)
		);

	auto value = builder.createLoad(gep);

	auto comparison = builder.createCmp(llvm::CmpInst::ICMP_SLT, index, length);

	builder.createCondBr(comparison , ifTrue, ifFalse);

	auto epIfFalse = iteratorEntryPoint
		.withBlock(ifFalse);

	auto epIfTrue = iteratorEntryPoint
		.withBlock(ifTrue);

	auto continuation = new ArrayContinuationNode(node->m_index, node->m_node, IteratorType::instance());
	auto expandable = new ExpandableValue(false, IteratorType::instance(), iteratorEntryPoint, continuation);

	auto tuple = new TupleValue({ expandable, value });

	return
	{
		{ epIfTrue, tuple },
		{ epIfFalse, value },
	};
}
