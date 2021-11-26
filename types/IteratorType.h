#ifndef ITERATORTYPE_H
#define ITERATORTYPE_H

#include <sstream>

#include "Type.h"
#include "Utility.h"

class IIteratorType : public Type
{
	public:
		virtual Type *elementType() const = 0;
};

class IteratorType : public Type
{
	public:
		static Type *instance()
		{
			static IteratorType instance;

			return &instance;
		}

		std::string tag() const override
		{
			return "...";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt8PtrTy(context);
		}

		bool is(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			return tag() == type->tag();
		}
};

#endif // ITERATORTYPE_H
