#include "default_prototype.hpp"
#include "ast.hpp"
#include "emitter.hpp"
#include "entry_point.hpp"
#include "structure_type.hpp"

#include "ast/struct_node.hpp"

#include <numeric>

namespace fluenc::code_generation::types
{
	std::pair<entry_point, std::vector<prototype_field>> default_fields(
		const entry_point& entryPoint,
		const emitter& visitor,
		const struct_node* prototype
	)
	{
		std::vector<prototype_field> fields;

		auto accumulated_entry_point = std::accumulate(
			begin(prototype->fields),
			end(prototype->fields),
			entryPoint,
			[&](auto current_entry_point, auto field) -> entry_point {
				if (auto default_value = field.default_value)
				{
					auto default_results = accept(*default_value, visitor, { current_entry_point, value_stack() });

					auto& [default_entry_point, default_values] = *default_results.begin();

					fields.push_back({ field.name, default_values.pop(), type_for(field.type, default_entry_point) });

					return default_entry_point;
				}
				else
				{
					fields.push_back({ field.name, nullptr, type_for(field.type, current_entry_point) });

					return current_entry_point;
				}
			}
		);

		auto final_entry_point = std::accumulate(
			begin(prototype->parent_types),
			end(prototype->parent_types),
			accumulated_entry_point,
			[&](auto aep, auto type) {
				auto prototype = static_cast<const types::structure_type*>(type_for(type, aep));
				auto [parent_entry_point, parent_fields] = default_fields(aep, visitor, prototype->subject());

				fields.insert(end(fields), begin(parent_fields), end(parent_fields));

				return parent_entry_point;
			}
		);

		return { final_entry_point, fields };
	}
}
