#include "junction_fragment.hpp"
#include "ast.hpp"
#include "emitter.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

namespace fluenc::code_generation::fragments
{
	junction_fragment::junction_fragment(const fluenc::expression_t& subject)
		: subject_(subject)
	{
	}

	std::vector<emitter_result> junction_fragment::build(const emitter& visitor, emitter_context context) const
	{
		struct type_comparer
		{
			bool operator()(const base_type* x, const base_type* y) const
			{
				if (x == y)
				{
					return false;
				}

				if (x->id() == type_id::tuple)
				{
					return true;
				}

				if (y->id() == type_id::tuple)
				{
					return false;
				}

				return std::less<const base_type*>()(x, y);
			}
		};

		struct SingleResult
		{
			const entry_point entryPoint;
			const base_value* value;
		};

		auto join = [&](const std::vector<SingleResult>& range, const entry_point& entryPoint) -> const base_value* {
			auto [_, first] = *range.begin();

			auto alloc = first->clone(entryPoint, clone_strategy::storage);

			for (auto& [resultEntryPoint, value] : range)
			{
				auto transferEntryPoint = value_helper::transfer_value(resultEntryPoint, value, alloc, visitor);

				link_blocks(transferEntryPoint.block(), entryPoint.block());
			}

			return alloc;
		};

		auto tryJoin = [&](const std::vector<SingleResult>& values, const entry_point& entryPoint) -> SingleResult {
			if (values.size() > 1)
			{
				auto context = entryPoint.context();

				auto junctionBlock = llvm::BasicBlock::Create(*context, "junction");

				auto junctionEntryPoint = entryPoint.with_block(junctionBlock);

				auto joined = join(values, junctionEntryPoint);

				return { junctionEntryPoint, joined };
			}

			return values[0];
		};

		auto inputResults = accept(subject_, visitor, context);

		std::vector<emitter_result> outputResults;

		std::map<const base_type*, std::vector<SingleResult>, type_comparer> groupedResults;

		for (auto& result : inputResults)
		{
			auto [resultEntryPoint, resultValues] = result;

			if (resultValues.size() != 1)
			{
				outputResults.push_back(result);

				continue;
			}

			auto value = resultValues.pop();

			groupedResults[value->type()].push_back({ resultEntryPoint, value });
		}

		for (auto& [type, inputValues] : groupedResults)
		{
			auto [joinedEntryPoint, joinedValue] = tryJoin(inputValues, context.entryPoint);

			outputResults.push_back({ joinedEntryPoint, joinedValue });
		}

		return outputResults;
	}
}
