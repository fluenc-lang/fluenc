#ifndef DZENTRYPOINTNG_H
#define DZENTRYPOINTNG_H

#include "DzCallable.h"

class DzEntryPointNg : public DzCallable
{
	public:
		DzEntryPointNg(DzValueNg *block
			, DzTypeNameNg *returnType
			, const std::string &name
			, const std::vector<DzMemberNg *> &arguments
			);

		std::string name() const override;

		std::vector<DzMemberNg *> arguments() const override;

		FunctionAttribute attribute() const override;

		DzTypeNameNg *returnType() const override;

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		DzValueNg *m_block;
		DzTypeNameNg *m_returnType;

		std::string m_name;
		std::vector<DzMemberNg *> m_arguments;
};

#endif // DZENTRYPOINTNG_H
