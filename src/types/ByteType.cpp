#include "types/ByteType.h"

std::string ByteType::name() const
{
	return "byte";
}

llvm::Type *ByteType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8Ty(context);
}
