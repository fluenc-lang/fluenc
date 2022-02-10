#ifndef DZITERATORFUNCTION_H
#define DZITERATORFUNCTION_H

#include "DzCallable.h"

class DzBaseArgument;
class DzFunction;
class Type;

class DzIteratorFunction : public DzCallable
{
	public:
		DzIteratorFunction(const std::vector<DzBaseArgument *> &arguments
			, const Type *iteratorType
			, const DzFunction *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

		std::string name() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

	private:
		std::vector<DzBaseArgument *> m_arguments;

		const Type *m_iteratorType;
		const DzFunction *m_subject;
};

#endif // DZITERATORFUNCTION_H
