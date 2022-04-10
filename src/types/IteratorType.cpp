#include "types/IteratorType.h"
#include "types/ArrayType.h"

Type *IteratorType::instance()
{
	static IteratorType instance;

	return &instance;
}

std::string IteratorType::name() const
{
	return "...";
}

llvm::Type *IteratorType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

int8_t IteratorType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	if (auto other = dynamic_cast<const IteratorType *>(type))
	{
		if (other == this)
		{
			return 0;
		}

		return 1;
	}

	if (auto array = dynamic_cast<const ArrayType *>(type))
	{
		return 0; // For now
	}

	return -1;
}
