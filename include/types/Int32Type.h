#ifndef INT32TYPE_H
#define INT32TYPE_H

#include "BuiltinType.h"

class Int32Type : public BuiltinType<Int32Type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // INT32TYPE_H
