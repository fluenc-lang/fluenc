#include "TypeCompatibilityCalculator.h"
#include "ITypeName.h"

#include "types/AnyType.h"
#include "types/AggregateType.h"
#include "types/ArrayType.h"
#include "types/IteratorType.h"
#include "types/ExpandedType.h"
#include "types/FunctionType.h"
#include "types/Prototype.h"
#include "types/WithPrototype.h"
#include "types/OpaquePointerType.h"
#include "types/PlaceholderType.h"
#include "types/ProxyType.h"
#include "types/UserType.h"
#include "types/BuiltinType.h"

#include "iterators/ExtremitiesIterator.h"

struct Storage
{
	Storage()
	{
		add<StringType, StringType>([](const EntryPoint &, auto, auto)
		{
			return 0;
		});

		add<StringType, IteratorType>([](const EntryPoint &, auto, auto)
		{
			return 1;
		});

		add<ArrayType, ArrayType>([](const EntryPoint &, auto left, auto right)
		{
			if (left == right)
			{
				return 0;
			}

			return -1;
		});

		add<ArrayType, IteratorType>([](const EntryPoint &, auto, auto)
		{
			return 0;
		});

		add<UserType, UserType>([](const EntryPoint &entryPoint, const auto left, auto right) -> int8_t
		{
			auto leftElementTypes = left->elementTypes();
			auto rightElementTypes = right->elementTypes();

			if (leftElementTypes.size() != rightElementTypes.size())
			{
				return -1;
			}

			auto prototypeCompatibility = TypeCompatibilityCalculator::calculate(entryPoint, left->prototype(), right->prototype());

			if (prototypeCompatibility < 0)
			{
				return prototypeCompatibility;
			}

			int8_t min = 0;
			int8_t max = 0;

			std::transform(begin(leftElementTypes), end(leftElementTypes), begin(rightElementTypes), extremities_iterator(min, max), [&](auto left, auto right)
			{
				return TypeCompatibilityCalculator::calculate(entryPoint, left, right);
			});

			if (min < 0 || max > 0)
			{
				return prototypeCompatibility;
			}

			return 0;
		});

		add<UserType, Type>([](const EntryPoint &entryPoint, const auto left, auto right)
		{
			return TypeCompatibilityCalculator::calculate(entryPoint, left->prototype(), right);
		});

		add<TupleType, TupleType>([](const EntryPoint &entryPoint, const auto left, auto right) -> int8_t
		{
			if (left == right)
			{
				return 0;
			}

			auto leftTypes = left->types();
			auto rightTypes = right->types();

			if (leftTypes.size() != rightTypes.size())
			{
				return -1;
			}

			int8_t min = 0;
			int8_t max = 0;

			std::transform(begin(leftTypes), end(leftTypes), begin(rightTypes), extremities_iterator(min, max), [&](auto left, auto right)
			{
				return TypeCompatibilityCalculator::calculate(entryPoint, left, right);
			});

			if (min < 0)
			{
				return min;
			}

			return max;
		});

		add<ProxyType, Type>([](const EntryPoint &, const auto, auto)
		{
			return -1;
		});

		add<PlaceholderType, Type>([](const EntryPoint &, const auto, auto)
		{
			return 0;
		});

		add<OpaquePointerType, OpaquePointerType>([](const EntryPoint &entryPoint, const auto left, auto right) -> int8_t
		{
			if (left->name() == right->name())
			{
				return 0;
			}

			return TypeCompatibilityCalculator::calculate(entryPoint, left->subject(), right->subject());
		});

		add<OpaquePointerType, Type>([](const EntryPoint &entryPoint, const auto left, auto right)
		{
			return TypeCompatibilityCalculator::calculate(entryPoint, left->subject(), right);
		});

		add<IteratorType, IteratorType>([](const EntryPoint &, const auto left, auto right)
		{
			if (left == right)
			{
				return 0;
			}

			return 1;
		});

		add<WithPrototype, Type>([](const EntryPoint &entryPoint, const auto left, auto right)
		{
			return TypeCompatibilityCalculator::calculate(entryPoint, left->value()->type(), right);
		});

		add<Prototype, AnyType>([](const EntryPoint &entryPoint, auto left, auto) -> int8_t
		{
			auto solve = [&](int8_t score, const Prototype *type, const EntryPoint &entryPoint, auto solve) -> int8_t
			{
				auto parentTypes = type->parentTypes();

				return std::accumulate(begin(parentTypes), end(parentTypes), score, [=](auto accumulated, auto parentType) -> int8_t
				{
					auto resolvedType = parentType->resolve(entryPoint);
					auto prototype = static_cast<const Prototype *>(resolvedType);

					return std::max(accumulated, solve(score + 1, prototype, entryPoint, solve));
				});
			};

			return solve(2, left, entryPoint, solve);
		});

		add<Prototype, Type>([](const EntryPoint &entryPoint, auto left, auto right) -> int8_t
		{
			auto solve = [&](int8_t score, const Prototype *type, const EntryPoint &entryPoint, auto solve) -> std::pair<bool, int8_t>
			{
				if (right->name() == type->name())
				{
					return { true, score };
				}

				auto parentTypes = type->parentTypes();

				return std::accumulate(begin(parentTypes), end(parentTypes), std::make_pair(false, score), [=](auto accumulated, auto parentType) -> std::pair<bool, int8_t>
				{
					auto resolvedType = parentType->resolve(entryPoint);
					auto prototype = static_cast<const Prototype *>(resolvedType);

					auto [accumulatedMatch, accumulatedScore] = accumulated;
					auto [currentMatch, currentScore] = solve(score + 1, prototype, entryPoint, solve);

					if (!accumulatedMatch)
					{
						return { currentMatch, currentScore };
					}

					if (!currentMatch)
					{
						return { accumulatedMatch, accumulatedScore };
					}

					return { true, std::min(currentScore, accumulatedScore) };
				});
			};

			auto [match, score] = solve(1, left, entryPoint, solve);

			if (match)
			{
				return score;
			}

			return -1;
		});

		add<FunctionType, FunctionType>([](const EntryPoint &entryPoint, auto left, auto right) -> int8_t
		{
			if (left == right)
			{
				return 0;
			}

			auto leftTypes = left->types();
			auto rightTypes = right->types();

			if (leftTypes.size() != rightTypes.size())
			{
				return -1;
			}

			int8_t min = 0;
			int8_t max = 0;

			std::transform(begin(leftTypes), end(leftTypes), begin(rightTypes), extremities_iterator(min, max), [&](auto left, auto right)
			{
				return TypeCompatibilityCalculator::calculate(entryPoint, left, right);
			});

			if (min < 0)
			{
				return min;
			}

			if (left->function() != right->function())
			{
				return std::max(static_cast<int8_t>(1), max);
			}

			return max;
		});

		add<ExpandedType, ExpandedType>([](const EntryPoint &entryPoint, auto left, auto right) -> int8_t
		{
			if (left == right)
			{
				return 0;
			}

			auto leftTypes = left->types();
			auto rightTypes = right->types();

			if (leftTypes.size() != rightTypes.size())
			{
				return -1;
			}

			int8_t min = 0;
			int8_t max = 0;

			std::transform(begin(leftTypes), end(leftTypes), begin(rightTypes), extremities_iterator(min, max), [&](auto left, auto right)
			{
				return TypeCompatibilityCalculator::calculate(entryPoint, left, right);
			});

			if (min < 0)
			{
				return min;
			}

			return max;
		});

		add<ExpandedType, IteratorType>([](const EntryPoint &, auto, auto)
		{
			return 1;
		});

		add<AggregateType, Type>([](const EntryPoint &entryPoint, auto left, auto right) -> int8_t
		{
			if (left == right)
			{
				return 0;
			}

			auto subjects = left->subjects();

			return std::accumulate(begin(subjects), end(subjects), static_cast<int8_t>(-1), [&](auto score, auto subject)
			{
				auto result = TypeCompatibilityCalculator::calculate(entryPoint, subject, right);

				if (result < 0)
				{
					return score;
				}

				if (score < 0)
				{
					return result;
				}

				return std::min(result, score);
			});
		});

		add<AnyType, IBuiltinType>([](const EntryPoint &, auto, auto)
		{
			return 1;
		});
	}

	template<typename TLeft, typename TRight>
	void add(TypeCompatibilityCalculator::calculator_t<TLeft, TRight> calculator)
	{
		auto strategy = new TypeCompatibilityCalculator::Strategy<TLeft, TRight>(calculator);

		calculators.push_back(strategy);
	}

	std::vector<TypeCompatibilityCalculator::IStrategy *> calculators;
};

int8_t TypeCompatibilityCalculator::calculate(const EntryPoint &entryPoint, const Type *left, const Type *right)
{
	static Storage storage;

	for (auto &strategy : storage.calculators)
	{
		if (auto result = strategy->calculate(entryPoint, left, right))
		{
			return *result;
		}
	}

	if (left == right)
	{
		return 0;
	}

	if (left->name() == right->name())
	{
		return 1;
	}

	return -1;
}
