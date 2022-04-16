#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include <numeric>

#include "Node.h"
#include "TokenInfo.h"

#include "values/UserTypeValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"

class FunctionCallNode : public Node
{
	public:
		FunctionCallNode(antlr4::ParserRuleContext *context, const std::vector<std::string> &names);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const CallableNode *findFunction(const EntryPoint &entryPoint, Stack values) const;

		template<typename Container>
		const EntryPoint *findTailCallTarget(const EntryPoint *candidate, Container container) const
		{
			return std::accumulate(container.begin(), container.end(), candidate, [&](const EntryPoint *target, auto value) -> const EntryPoint *
			{
				if (!target)
				{
					return nullptr;
				}

				if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
				{
					auto fields = userTypeValue->fields();

					std::vector<const BaseValue *> values;

					std::transform(begin(fields), end(fields), std::back_inserter(values), [](auto field)
					{
						return field->value();
					});

					return findTailCallTarget(target, values);
				}

				if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
				{
					return findTailCallTarget(target
						, tupleValue->values()
						);
				}

				if (auto expandedValue = dynamic_cast<const ExpandedValue *>(value))
				{
					auto provider = expandedValue->provider();

					if (provider->depth() < target->depth())
					{
						return provider->entry();
					}
				}

				return target;
			});
		}

		std::vector<DzResult> regularCall(const EntryPoint &entryPoint, Stack values) const;

		TokenInfo m_token;

		std::vector<std::string> m_names;
};

#endif // FUNCTIONCALLNODE_H
