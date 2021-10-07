#ifndef WITHOUTTYPE_H
#define WITHOUTTYPE_H

#include "BuiltinType.h"

class WithoutType : public BuiltinType<WithoutType>
{
	public:
		std::string tag() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // WITHOUTTYPE_H
