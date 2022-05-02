#ifndef FUNCTIONCALLPROXYNODE_H
#define FUNCTIONCALLPROXYNODE_H

#include <numeric>

#include "Node.h"

#include "values/UserTypeValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"
#include "values/NamedValue.h"

class FunctionCallProxyNode : public Node
{
	public:
		FunctionCallProxyNode(const std::vector<std::string> &names, const Node *regularCall);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		int8_t tryCreateTailCall(const EntryPoint &entryPoint
			, const Stack &values
			, const std::vector<std::string>::const_iterator &name
			, const std::vector<std::string>::const_iterator &end
			) const;

		template<typename Container>
		const EntryPoint *findTailCallTarget(const EntryPoint *candidate, Container container) const
		{
			return std::accumulate(container.begin(), container.end(), candidate, [&](const EntryPoint *target, auto value) -> const EntryPoint *
			{
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

		std::vector<std::string> m_names;

		const Node *m_regularCall;
};

#endif // FUNCTIONCALLPROXYNODE_H
