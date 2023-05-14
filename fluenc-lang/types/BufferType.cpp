#include "BufferType.h"

TypeId BufferType::id() const
{
	return TypeId::Buffer;
}

std::string BufferType::name() const
{
	return "buffer";
}

llvm::Type *BufferType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
