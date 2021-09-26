#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class Type
{
	public:
		virtual std::string tag() const = 0;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;
};

#endif // TYPE_H
