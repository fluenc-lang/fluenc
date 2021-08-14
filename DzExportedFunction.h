#ifndef DZEXPORTEDFUNCTION_H
#define DZEXPORTEDFUNCTION_H

#include "DzCallable.h"

class DzTypeName;

class DzExportedFunction : public DzCallable
{
	public:
		DzExportedFunction(const std::string &name, DzValue *block, DzTypeName *returnType);

		std::string name() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values, size_t numberOfArguments) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		DzValue *m_block;
		DzTypeName *m_returnType;
};

#endif // DZEXPORTEDFUNCTION_H
