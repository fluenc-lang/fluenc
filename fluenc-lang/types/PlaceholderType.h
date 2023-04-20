#ifndef PLACEHOLDERTYPE_H
#define PLACEHOLDERTYPE_H

#include "Type.h"

class PlaceholderType : public Type
{
	public:
		TypeId id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // PLACEHOLDERTYPE_H
