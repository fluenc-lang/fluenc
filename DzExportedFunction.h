#ifndef DZEXPORTEDFUNCTION_H
#define DZEXPORTEDFUNCTION_H

#include "DzCallable.h"

class DzExportedFunction : public DzCallable
{
	public:
		DzExportedFunction(const std::string &name, DzValue *block);

		std::string name() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values, size_t numberOfArguments) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		DzValue *m_block;
};

#endif // DZEXPORTEDFUNCTION_H
