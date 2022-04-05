#ifndef IMPORTEDFUNCTION_H
#define IMPORTEDFUNCTION_H

#include "nodes/Callable.h"

class DzBaseArgument;
class ITypeName;

class ImportedFunction : public Callable
{
	public:
		ImportedFunction(const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, ITypeName *returnType
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		ITypeName *m_returnType;
};

#endif // IMPORTEDFUNCTION_H
