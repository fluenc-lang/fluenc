#include "types/ByteType.h"

std::string ByteType::name() const
{
	return "u8";
}

llvm::Type *ByteType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8Ty(context);
}
