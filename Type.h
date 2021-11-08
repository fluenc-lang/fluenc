#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class EntryPoint;

enum class TypeKind
{
	Builtin,
	Prototype,
	UserType,
	Iterator,
};

class Type
{
	public:
		virtual TypeKind kind() const = 0;

		virtual std::string tag() const = 0;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;

		virtual Type *iteratorType() const = 0;

		virtual bool is(const Type *type, const EntryPoint &entryPoint) const = 0;
};

#endif // TYPE_H
