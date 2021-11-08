#ifndef DZFUNCTION_H
#define DZFUNCTION_H

#include "DzCallable.h"

class DzArgument;

class DzFunction : public DzCallable
{
	public:
		DzFunction(FunctionAttribute attribute
			, const std::string &name
			, const std::vector<DzArgument *> &arguments
			, DzValue *block
			);

		std::string name() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		FunctionAttribute m_attribute;

		std::string m_name;
		std::vector<DzArgument *> m_arguments;

		DzValue *m_block;
};


#endif // DZFUNCTION_H
