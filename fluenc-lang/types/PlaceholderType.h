#ifndef PLACEHOLDERTYPE_H
#define PLACEHOLDERTYPE_H

#include "Type.h"

class PlaceholderType : public Type
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;
};

#endif // PLACEHOLDERTYPE_H
