#ifndef DZMEMBERACCESSNG_H
#define DZMEMBERACCESSNG_H

#include "DzValueNg.h"

class DzMemberAccessNg : public DzValueNg
{
	public:
		DzMemberAccessNg(const std::string &name);

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		std::string m_name;
};

#endif // DZMEMBERACCESSNG_H
