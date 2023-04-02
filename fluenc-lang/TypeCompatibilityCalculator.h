#ifndef TYPECOMPATIBILITYCALCULATOR_H
#define TYPECOMPATIBILITYCALCULATOR_H

#include <typeindex>
#include <map>
#include <optional>

#include "Type.h"

struct TypeCompatibilityCalculator
{
	template<typename TLeft, typename TRight>
	using calculator_t = std::function<int8_t(const EntryPoint &, const TLeft *, const TRight *)>;

	struct IStrategy
	{
		public:
			virtual std::optional<int8_t> calculate(const EntryPoint &entryPoint, const Type *left, const Type *right) const = 0;
	};

	template<typename TLeft, typename TRight>
	class Strategy : public IStrategy
	{
		public:
			Strategy(calculator_t<TLeft, TRight> callback)
				: m_callback(callback)
			{
			}

			std::optional<int8_t> calculate(const EntryPoint &entryPoint, const Type *left, const Type *right) const override
			{
				std::vector<const Type *> types = { left, right };

				for (auto i = 0; i < 2; i++)
				{
					auto castedLeft = dynamic_cast<const TLeft *>(types[0]);
					auto castedRight = dynamic_cast<const TRight *>(types[1]);

					if (castedLeft && castedRight)
					{
						return m_callback(entryPoint, castedLeft, castedRight);
					}

					std::reverse(begin(types), end(types));
				}

				return {};
			}

		private:
			calculator_t<TLeft, TRight> m_callback;
	};

	static int8_t calculate(const EntryPoint &entryPoint, const Type *left, const Type *right);
};

#endif // TYPECOMPATIBILITYCALCULATOR_H
