#include <numeric>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "entry_point.hpp"
#include "interop_helper.hpp"
#include "ir_builder.hpp"

#include "iterators/index_iterator.hpp"
#include "iterators/void_iterator.hpp"

#include "types/opaque_pointer_type.hpp"
#include "types/proxy_type.hpp"
#include "types/structure_type.hpp"

#include "values/buffer_value.hpp"
#include "values/named_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/user_type_value.hpp"

#include "exceptions/missing_type_declaration_exception.hpp"

namespace fluenc::code_generation
{
	read_proxy interop_helper::create_read_proxy(
		llvm::Value* value,
		const base_type* type,
		const entry_point& entry_point,
		const std::shared_ptr<peg::Ast>& ast
	)
	{
		auto context = entry_point.context();

		auto block = entry_point.block();

		ir_builder builder(entry_point);

		if (auto structure = type_cast<const types::structure_type*>(type))
		{
			auto fields = structure->fields(entry_point);

			if (empty(fields))
			{
				return { entry_point, new values::scalar_value { types::opaque_pointer_type::get(structure), value } };
			}

			std::vector<llvm::Type*> types;

			std::transform(begin(fields), end(fields), std::back_inserter(types), [&](auto field) {
				return field.type->storage_type(*context);
			});

			auto intType = llvm::Type::getInt32Ty(*context);
			auto structType = llvm::StructType::get(*context, types);
			auto structPtr = structType->getPointerTo();

			auto cast = builder.create_bit_cast(value, structPtr, "cast");

			std::vector<const values::named_value*> field_values;

			auto [accumulatedEntryPoint, _] = std::
				accumulate(begin(fields), end(fields), std::make_pair(entry_point, 0), [&](auto pair, auto field) {
					auto [lastEntryPoint, index] = pair;

					if (field.type)
					{
						llvm::Value* indexes[] = { llvm::ConstantInt::get(intType, 0),
												   llvm::ConstantInt::get(intType, index) };

						auto gep = new values::reference_value(
							field.type,
							llvm::GetElementPtrInst::CreateInBounds(structType, cast, indexes, field.name, block)
						);

						auto load = builder.create_load(gep, field.name);

						auto [result_entry_point, value] = create_read_proxy(*load, field.type, lastEntryPoint, ast);

						auto named_value = new values::named_value { field.name, value };

						field_values.push_back(named_value);

						return std::make_pair(result_entry_point, index + 1);
					}

					throw missing_type_declaration_exception(ast, type->name(), field.name);
				});

			return { accumulatedEntryPoint, new values::user_type_value { structure, field_values } };
		}
		else if (type->id() == type_id::buffer)
		{
			auto alloc = entry_point.alloc(type);

			ir_builder builder(entry_point);

			auto address = new values::scalar_value(type, value);

			builder.create_store(address, alloc);

			return { entry_point, new values::buffer_value(alloc) };
		}

		return { entry_point, new values::scalar_value { type, value } };
	}

	llvm::Value* interop_helper::create_write_proxy(const values::user_type_value *userTypeValue, const entry_point& entryPoint)
	{
		auto context = entryPoint.context();
		auto module = entryPoint.module();

		auto block = entryPoint.block();

		ir_builder builder(entryPoint);

		auto data_layout = module->getDataLayout();

		auto fields = userTypeValue->fields();

		std::vector<const values::scalar_value*> element_values;

		std::transform(
			begin(fields),
			end(fields),
			std::back_inserter(element_values),
			[&](const values::named_value* field) -> const values::scalar_value* {
				auto fieldValue = field->value();

				if (auto reference = value_cast<const values::reference_value*>(fieldValue))
				{
					return builder.create_load(reference, field->name());
				}

				if (auto user_type_value = value_cast<const values::user_type_value*>(fieldValue))
				{
					return new values::scalar_value { user_type_value->type(),
													  create_write_proxy(user_type_value, entryPoint) };
				}

				throw std::exception();
			}
		);

		std::vector<llvm::Type*> element_types;

		std::transform(begin(element_values), end(element_values), std::back_inserter(element_types), [&](auto value) {
			auto type = value->type();

			return type->storage_type(*context);
		});

		auto int_type = llvm::Type::getInt32Ty(*context);
		auto struct_type = llvm::StructType::get(*context, element_types);

		auto proxy_type = new types::proxy_type(struct_type);

		auto alloc = entryPoint.alloc(proxy_type);

		std::transform(begin(element_values), end(element_values), index_iterator(), void_iterator<llvm::Value*>(), [&](auto field, auto index) {
			llvm::Value* indexes[] = { llvm::ConstantInt::get(int_type, 0), llvm::ConstantInt::get(int_type, index) };

			auto gep = new values::reference_value(
				field->type(),
				llvm::GetElementPtrInst::CreateInBounds(struct_type, *alloc, indexes, "gep", block)
			);

			return builder.create_store(field, gep);
		});

		return builder.create_bit_cast(*alloc, llvm::Type::getInt8PtrTy(*context), "cast");
	}
}
