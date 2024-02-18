#include "string_iterator_fragment.hpp"
#include "array_continuation_fragment.hpp"
#include "emitter.hpp"
#include "index_repeater_fragment.hpp"
#include "ir_builder.hpp"
#include "utility.hpp"

#include "values/expandable_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/tuple_value.hpp"

#include "types/byte_type.hpp"
#include "types/int32_type.hpp"
#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"

#include <llvm/IR/Constants.h>

namespace fluenc::code_generation::fragments
{
	string_iterator_fragment::string_iterator_fragment(
		const values::reference_value* index,
		const values::reference_value* address,
		const values::reference_value* length
	)
		: index_(index)
		, node_(new index_repeater_fragment(this))
		, address_(address)
		, length_(length)
	{
	}

	std::vector<emitter_result> string_iterator_fragment::build(const emitter& visitor, emitter_context context) const
	{
		UNUSED(visitor);

		auto llvm_context = context.entryPoint.context();
		auto module = context.entryPoint.module();

		auto block = context.entryPoint.block();

		auto iterator_block = llvm::BasicBlock::Create(*llvm_context, "iterator");

		link_blocks(block, iterator_block);

		auto iterator_entry_point = context.entryPoint.with_block(iterator_block).with_name("__iterator").mark_entry();

		ir_builder builder(iterator_entry_point);

		auto index_type = index_->type();
		auto storage_type = index_type->storage_type(*llvm_context);

		auto if_true = create_block(llvm_context);
		auto if_false = create_block(llvm_context);

		auto length_type = types::int64_type::instance();
		auto length_storage_type = length_type->storage_type(*llvm_context);

		auto length_load = builder.create_load(length_);

		auto constant_one = new values::scalar_value(length_type, llvm::ConstantInt::get(length_storage_type, 1));

		auto length = builder.create_sub(length_load, constant_one);
		auto index = builder.create_load(index_);

		auto character_type = llvm::Type::getInt8Ty(*llvm_context);
		auto string_type = llvm::Type::getInt8PtrTy(*llvm_context);

		auto data_layout = module->getDataLayout();

		auto align = data_layout.getABITypeAlign(storage_type);

		auto load = new llvm::LoadInst(string_type, *address_, "strLoad", false, align, iterator_block);

		auto gep = new values::reference_value(
			types::byte_type::instance(),
			llvm::GetElementPtrInst::CreateInBounds(character_type, load, { *index }, "stringAccess", iterator_block)
		);

		auto value = builder.create_load(gep);

		auto target_type = types::int32_type::instance();

		auto sext = new values::scalar_value(
			target_type,
			new llvm::SExtInst(*value, target_type->storage_type(*llvm_context), "sext", iterator_block)
		);

		auto comparison = builder.create_cmp(llvm::CmpInst::ICMP_SLT, index, length);

		builder.create_cond_br(comparison, if_true, if_false);

		auto ep_if_false = iterator_entry_point.with_block(if_false);

		auto ep_if_true = iterator_entry_point.with_block(if_true);

		auto continuation = new fragments::array_continuation_fragment(index_, node_, types::iterator_type::instance());
		auto expandable = new values::
			expandable_value(false, types::iterator_type::instance(), iterator_entry_point, continuation, index_);

		auto tuple = new values::tuple_value({ expandable, sext });

		return {
			{ ep_if_true, tuple },
			{ ep_if_false, sext },
		};
	}
}
