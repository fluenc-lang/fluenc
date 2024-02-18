#ifndef PLACEHOLDERTYPE_H
#define PLACEHOLDERTYPE_H

#include "Type.h"

class placeholder_type : public Type
{
	public:
        type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // PLACEHOLDERTYPE_H
