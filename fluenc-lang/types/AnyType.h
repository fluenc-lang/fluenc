#ifndef ANYTYPE_H
#define ANYTYPE_H

#include "Type.h"

class AnyType : public Type
{
	public:
		static Type *instance();

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // ANYTYPE_H
