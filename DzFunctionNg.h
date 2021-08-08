#ifndef DZFUNCTIONNG_H
#define DZFUNCTIONNG_H

#include "DzCallable.h"

class DzFunctionNg : public DzCallable
{
	public:
		DzFunctionNg(DzValueNg *block
			, const std::string &name
			, const std::vector<DzMemberNg *> arguments
			);

		std::string name() const override;

		std::vector<DzMemberNg *> arguments() const override;

		FunctionAttribute attribute() const override;

		DzTypeNameNg *returnType() const override;

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		std::string m_name;
		std::vector<DzMemberNg *> m_arguments;

		DzValueNg *m_block;
};

#endif // DZFUNCTIONNG_H
