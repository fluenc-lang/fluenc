#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "BufferValue.h"
#include "IRBuilderEx.h"
#include "LazyValue.h"
#include "ReferenceValue.h"
#include "ScalarValue.h"
#include "StringIteratableGenerator.h"
#include "utility.hpp"

#include "types/BufferType.h"
#include "types/Int64Type.h"

BufferValue::BufferValue(const reference_value *address)
	: m_address(address)
{

}

value_id BufferValue::id() const
{
    return value_id::buffer;
}

const reference_value *BufferValue::address() const
{
	return m_address;
}

const reference_value *BufferValue::reference(const EntryPoint &entryPoint) const
{
	auto llvmContext = entryPoint.context();
	auto block = entryPoint.block();

	auto elementType = llvm::Type::getInt8Ty(*llvmContext);

	auto intType = llvm::Type::getInt32Ty(*llvmContext);

	llvm::Value *indexes[] =
	{
		llvm::ConstantInt::get(intType, sizeof(size_t))
	};

	IRBuilderEx builder(entryPoint);

	auto buffer = builder.createLoad(m_address);

        return new reference_value(buffer->type()
		, llvm::GetElementPtrInst::CreateInBounds(elementType, *buffer, indexes, "bufferLoad", block)
		);
}

const lazy_value *BufferValue::iterator(const EntryPoint &entryPoint) const
{
	IRBuilderEx builder(entryPoint);

	auto buffer = builder.createLoad(m_address);

        auto length = new reference_value(int64_type::instance(), *buffer);

	// Create a shared pointer, so that we can reuse the logic of
	// StringIteratableGenerator & co. It usually expects a pointer
	// to a string.
	//
	auto address = reference(entryPoint);
        auto value = new scalar_value(address->type(), *address);

	auto sharedPointer = entryPoint.alloc(address->type());

	builder.createStore(value, sharedPointer);

	auto generator = new StringIteratableGenerator(sharedPointer, length);

        return new lazy_value(generator, entryPoint);
}

const Type *BufferValue::type() const
{
    return buffer_type::instance();
}

const BaseValue *BufferValue::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}
