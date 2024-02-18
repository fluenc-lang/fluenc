#ifndef INT64TYPE_H
#define INT64TYPE_H

#include "IntegerType.h"

class int64_type : public IntegerType<int64_type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // INT64TYPE_H
