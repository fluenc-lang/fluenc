#ifndef DZIMPORTEDFUNCTION_H
#define DZIMPORTEDFUNCTION_H

#include "DzCallable.h"

class DzArgument;
class DzTypeName;

class DzImportedFunction : public DzCallable
{
	public:
		DzImportedFunction(const std::string &name
			, const std::vector<DzArgument *> &arguments
			, DzTypeName *returnType
			);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::string name() const override;

		FunctionAttribute attribute() const override;

		bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
		std::vector<DzArgument *> m_arguments;

		DzTypeName *m_returnType;
};

#endif // DZIMPORTEDFUNCTION_H
