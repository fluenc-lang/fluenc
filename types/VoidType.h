#ifndef VOIDTYPE_H
#define VOIDTYPE_H

#include "BuiltinType.h"

class VoidType : public BuiltinType<VoidType>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // VOIDTYPE_H
