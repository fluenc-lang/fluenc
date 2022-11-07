#ifndef INT64TYPE_H
#define INT64TYPE_H

#include "IntegerType.h"

class Int64Type : public IntegerType<Int64Type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // INT64TYPE_H
