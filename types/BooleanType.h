#ifndef BOOLEANTYPE_H
#define BOOLEANTYPE_H

#include "BuiltinType.h"

class BooleanType : public BuiltinType<BooleanType>
{
	public:
		std::string tag() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BOOLEANTYPE_H
