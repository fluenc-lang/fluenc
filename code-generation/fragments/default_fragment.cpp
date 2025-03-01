#include "default_fragment.hpp"
#include "emitter.hpp"
#include "entry_point.hpp"
#include "index_sink_fragment.hpp"
#include "indexed.hpp"
#include "terminator_fragment.hpp"

#include "types/aggregate_type.hpp"
#include "types/array_type.hpp"
#include "types/buffer_type.hpp"
#include "types/function_type.hpp"
#include "types/int64_type.hpp"
#include "types/string_type.hpp"
#include "types/structure_type.hpp"
#include "types/user_type.hpp"

#include "values/buffer_value.hpp"
#include "values/function_value.hpp"
#include "values/named_value.hpp"
#include "values/pre_lazy_value.hpp"
#include "values/reference_value.hpp"
#include "values/string_value.hpp"
#include "values/user_type_value.hpp"
#include "values/without_value.hpp"

#include "iterators/index_iterator.hpp"

#include "generators/array_iterator_generator.hpp"

#include <numeric>

#include <range/v3/view/zip.hpp>

namespace fluenc::code_generation::fragments
{
	default_fragment::default_fragment(const base_type* type, const fragment* consumer)
		: type_(type)
		, consumer_(consumer)
	{
	}

	struct AllocResult
	{
		const entry_point entryPoint;
		const base_value* value;
	};

	AllocResult alloc(const base_type* type, const emitter& visitor, const entry_point& entryPoint)
	{
		if (auto userType = type_cast<const types::user_type*>(type))
		{
			auto element_types = userType->element_types();
			auto prototype = userType->prototype();

			auto fields = prototype->fields(entryPoint);

			std::vector<const values::named_value*> values;

			auto zipped = ranges::views::zip(fields, element_types);

			auto accumulatedEntryPoint = std::
				accumulate(begin(zipped), end(zipped), entryPoint, [&](auto currentEntryPoint, auto pair) {
					auto& [field, elementType] = pair;

					auto [allocEntryPoint, allocValue] = alloc(elementType, visitor, currentEntryPoint);

					auto namedValue = new values::named_value(field.name, allocValue);

					values.push_back(namedValue);

					return allocEntryPoint;
				});

			return { accumulatedEntryPoint, new values::user_type_value(userType->prototype(), values) };
		}

		if (auto array = type_cast<const types::array_type*>(type))
		{
			auto types = array->types();

			std::vector<fluenc::indexed<const base_type*>> indexed;

			transform(begin(types), end(types), fluenc::index_iterator(0), back_inserter(indexed), [&](auto type, auto index) {
				return fluenc::indexed<const base_type*> { index, type };
			});

			auto array_contents = std::accumulate(
				rbegin(indexed),
				rend(indexed),
				std::vector<emitter_result> { { entryPoint, value_stack_t() } },
				[&](auto previous, fluenc::indexed<const base_type*> expression) {
					index_sink_fragment index_sink(expression.index, terminator_fragment::instance());

					std::vector<emitter_result> results;

					for (auto [previous_entry_point, values] : previous)
					{
						auto [alloc_entry_point, alloc_value] = alloc(expression.value, visitor, previous_entry_point);

						values.push(alloc_value);

						for (auto& result : index_sink.build(visitor, emitter_context { alloc_entry_point, values }))
						{
							results.push_back(result);
						}
					}

					return results;
				}
			);

			auto generator = new generators::array_iterator_generator(array_contents, {}, size(indexed));
			auto lazy = new values::pre_lazy_value(generator, entryPoint);

			return { entryPoint, lazy };
		}

		if (auto string = type_cast<const types::string_type*>(type))
		{
			auto alloc = entryPoint.alloc(string);
			auto length = entryPoint.alloc(types::int64_type::instance());

			return { entryPoint, new values::string_value(alloc, length) };
		}

		if (auto buffer = type_cast<const types::buffer_type*>(type))
		{
			auto alloc = entryPoint.alloc(buffer);

			return { entryPoint, new values::buffer_value(alloc) };
		}

		if (type->id() == type_id::without)
		{
			return { entryPoint, values::without_value::instance() };
		}

		if (auto aggregate = type_cast<const types::aggregate_type*>(type))
		{
			std::vector<fluenc::function_node*> functions;

			auto subjects = aggregate->subjects();

			std::transform(begin(subjects), end(subjects), back_inserter(functions), [](const base_type* subject) {
				if (auto function = type_cast<const types::function_type*>(subject))
				{
					return function->function();
				}
				else
				{
					throw invalid_type_exception(nullptr, "function", subject->name());
				}
			});

			return { entryPoint, new values::function_value(functions, entryPoint) };
		}

		return { entryPoint, entryPoint.alloc(type) };
	}

	std::vector<emitter_result> default_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto [entryPoint, value] = alloc(type_, visitor, context.entryPoint);

		context.values.push(value);

		return consumer_->build(visitor, { entryPoint, context.values });
	}
}
