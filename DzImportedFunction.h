#ifndef DZIMPORTEDFUNCTION_H
#define DZIMPORTEDFUNCTION_H

#include "DzCallable.h"

class DzBaseArgument;
class ITypeName;

class DzImportedFunction : public DzCallable
{
	public:
		DzImportedFunction(const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, ITypeName *returnType
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		ITypeName *m_returnType;
};

#endif // DZIMPORTEDFUNCTION_H
