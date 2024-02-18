#include "BufferType.h"

type_id buffer_type::id() const
{
    return type_id::Buffer;
}

std::string buffer_type::name() const
{
	return "buffer";
}

llvm::Type *buffer_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
