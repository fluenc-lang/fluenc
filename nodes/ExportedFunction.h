#ifndef DZEXPORTEDFUNCTION_H
#define DZEXPORTEDFUNCTION_H

#include "nodes/Callable.h"

class ITypeName;

class ExportedFunction : public Callable
{
	public:
		ExportedFunction(const std::string &name, Node *block, ITypeName *returnType);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		Node *m_block;
		ITypeName *m_returnType;
};

#endif // DZEXPORTEDFUNCTION_H
