#ifndef INT32TYPE_H
#define INT32TYPE_H

#include "IntegerType.h"

class int32_type : public IntegerType<int32_type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // INT32TYPE_H
