#ifndef ANYTYPE_H
#define ANYTYPE_H

#include "BuiltinType.h"

class AnyType : public BuiltinType<AnyType>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;
		bool equals(const Type *type, const EntryPoint &entryPoint) const override;
};

#endif // ANYTYPE_H
