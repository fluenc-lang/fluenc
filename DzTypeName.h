#ifndef DZTYPENAME_H
#define DZTYPENAME_H

#include <llvm/IR/Type.h>

class EntryPoint;

class DzTypeName
{
	public:
		DzTypeName(const std::string &name);

		std::string name() const;

		llvm::Type *resolve(const EntryPoint &entryPoint);

		static DzTypeName *int32();
		static DzTypeName *int64();
		static DzTypeName *boolean();
		static DzTypeName *string();

	private:
		std::string m_name;
};

#endif // DZTYPENAME_H
