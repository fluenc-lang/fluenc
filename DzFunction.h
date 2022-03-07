#ifndef DZFUNCTION_H
#define DZFUNCTION_H

#include "DzCallable.h"

class DzArgument;
class DzBaseArgument;
class BaseValue;

class DzFunction : public DzCallable
{
	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	public:
		DzFunction(FunctionAttribute attribute
			, const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, DzValue *block
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<Argument> handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, const BaseValue *value) const;

		FunctionAttribute m_attribute;

		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		DzValue *m_block;
};


#endif // DZFUNCTION_H
