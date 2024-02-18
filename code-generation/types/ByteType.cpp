#include "types/ByteType.h"

std::string byte_type::name() const
{
	return "u8";
}

llvm::Type *byte_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8Ty(context);
}
