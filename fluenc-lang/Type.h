#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class EntryPoint;
class IOperatorSet;

class Type
{
	public:
		virtual std::string name() const = 0;
		virtual std::string fullName() const;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;

		virtual IOperatorSet *operators() const;
};

#endif // TYPE_H
