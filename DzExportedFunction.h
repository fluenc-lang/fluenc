#ifndef DZEXPORTEDFUNCTION_H
#define DZEXPORTEDFUNCTION_H

#include "DzCallable.h"

class ITypeName;

class DzExportedFunction : public DzCallable
{
	public:
		DzExportedFunction(const std::string &name, DzValue *block, ITypeName *returnType);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		DzValue *m_block;
		ITypeName *m_returnType;
};

#endif // DZEXPORTEDFUNCTION_H
