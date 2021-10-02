#ifndef UINT32TYPE_H
#define UINT32TYPE_H

#include "BuiltinType.h"

class Uint32Type : public BuiltinType<Uint32Type>
{
	public:
		std::string tag() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // UINT32TYPE_H
