#include "buffer_value.hpp"
#include "ir_builder.hpp"
#include "pre_lazy_value.hpp"
#include "reference_value.hpp"
#include "scalar_value.hpp"
#include "utility.hpp"

#include "generators/string_iterator_generator.hpp"

#include "types/buffer_type.hpp"
#include "types/int64_type.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

namespace fluenc::code_generation::values
{
	buffer_value::buffer_value(const reference_value* address)
		: address_(address)
	{
	}

	value_id buffer_value::id() const
	{
		return value_id::buffer;
	}

	const reference_value* buffer_value::address() const
	{
		return address_;
	}

	const reference_value* buffer_value::reference(const entry_point& entryPoint) const
	{
		auto llvm_context = entryPoint.context();
		auto block = entryPoint.block();

		auto element_type = llvm::Type::getInt8Ty(*llvm_context);

		auto int_type = llvm::Type::getInt32Ty(*llvm_context);

		llvm::Value* indexes[] = { llvm::ConstantInt::get(int_type, sizeof(size_t)) };

		ir_builder builder(entryPoint);

		auto buffer = builder.create_load(address_);

		return new reference_value(
			buffer->type(),
			llvm::GetElementPtrInst::CreateInBounds(element_type, *buffer, indexes, "bufferLoad", block)
		);
	}

	const pre_lazy_value* buffer_value::iterator(const entry_point& entryPoint) const
	{
		ir_builder builder(entryPoint);

		auto buffer = builder.create_load(address_);

		auto length = new reference_value(types::int64_type::instance(), *buffer);

		// Create a shared pointer, so that we can reuse the logic of
		// string_iterator_generator & co. It usually expects a pointer
		// to a string.
		//
		auto address = reference(entryPoint);
		auto value = new scalar_value(address->type(), *address);

		auto shared_pointer = entryPoint.alloc(address->type());

		builder.create_store(value, shared_pointer);

		auto generator = new generators::string_iterator_generator(shared_pointer, length);

		return new values::pre_lazy_value(generator, entryPoint);
	}

	const base_type* buffer_value::type() const
	{
		return types::buffer_type::instance();
	}

	const base_value* buffer_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}
}
