#ifndef DZTYPENAMENG_H
#define DZTYPENAMENG_H

#include <string>

#include <llvm/IR/Type.h>

class EntryPointInfo;

class DzTypeNameNg
{
	public:
		DzTypeNameNg(const std::string &name);

		llvm::Type *build(const EntryPointInfo &entryPoint) const;

	private:
		std::string m_name;
};

#endif // DZTYPENAMENG_H
