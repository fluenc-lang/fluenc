#include "IRBuilderEx.h"

#include "values/StringIteratable.h"
#include "values/ReferenceValue.h"
#include "values/ScalarValue.h"
#include "values/TupleValue.h"
#include "values/ExpandableValue.h"

#include "types/Int32Type.h"

#include "nodes/ArrayContinuationNode.h"

StringIteratable::StringIteratable(const ReferenceValue *index, const Node *node, llvm::Value *address, size_t length)
	: m_index(index)
	, m_node(node)
	, m_address(address)
	, m_length(length)
{
}

std::vector<DzResult> StringIteratable::build(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

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

	auto indexType = m_index->type();
	auto storageType = indexType->storageType(*context);

	auto ifTrue = llvm::BasicBlock::Create(*context);
	auto ifFalse = llvm::BasicBlock::Create(*context);

	auto length = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, m_length - 1)
		);

	auto index = builder.createLoad(m_index);

	auto characterType = llvm::Type::getInt8Ty(*context);
	auto stringType = llvm::Type::getInt8PtrTy(*context);

	auto cast = new llvm::BitCastInst(m_address, stringType, "stringCast", iteratorBlock);

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

	auto continuation = new ArrayContinuationNode(m_index, m_node, IteratorType::instance());
	auto expandable = new ExpandableValue(IteratorType::instance(), iteratorEntryPoint, continuation);

	auto tuple = new TupleValue({ expandable, value });

	return
	{
		{ epIfTrue, tuple },
		{ epIfFalse, value },
	};
}
