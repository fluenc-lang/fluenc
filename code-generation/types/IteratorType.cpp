#include "types/IteratorType.h"

Type *iterator_type::instance()
{
	static iterator_type instance;

	return &instance;
}

type_id iterator_type::id() const
{
    return type_id::Iterator;
}

std::string iterator_type::name() const
{
	return "...";
}

llvm::Type *iterator_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

Type *iterator_type::get(const iterator *it)
{
	static std::unordered_map<const iterator*, iterator_type> cache;

	return &cache[it];
}
