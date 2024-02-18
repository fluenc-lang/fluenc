#include "IPrototype.h"
#include "EntryPoint.h"
#include "Emitter.h"
#include "structure_type.hpp"
#include "ast.hpp"

#include "ast/struct_node.hpp"

#include <numeric>

std::pair<EntryPoint, std::vector<structure_field_t>> default_fields(const EntryPoint &entryPoint, const Emitter &visitor, const fluenc::struct_node* prototype)
{
    std::vector<structure_field_t> fields;

	auto accumulatedEntryPoint = std::accumulate(begin(prototype->fields), end(prototype->fields), entryPoint, [&](auto currentEntryPoint, auto field) -> EntryPoint
	{
		auto defaultValue = field.default_value;

		if (defaultValue)
		{
			auto defaultResults = accept(*defaultValue, visitor, { currentEntryPoint, Stack() });

			auto &[defaultEntryPoint, defaultValues] = *defaultResults.begin();

			fields.push_back({ field.name, defaultValues.pop(), type_for(field.type, defaultEntryPoint) });

			return defaultEntryPoint;
		}
		else
		{
			fields.push_back({ field.name, nullptr, type_for(field.type, currentEntryPoint) });

			return currentEntryPoint;
		}
	});

	auto finalEntryPoint = std::accumulate(begin(prototype->parent_types), end(prototype->parent_types), accumulatedEntryPoint, [&](auto aep, auto type)
	{
		auto prototype = static_cast<const structure_type *>(type_for(type, aep));
		auto [parentEntryPoint, parentFields] = default_fields(aep, visitor, prototype->subject());

		fields.insert(end(fields), begin(parentFields), end(parentFields));

		return parentEntryPoint;
	});

	return { finalEntryPoint, fields };
}
