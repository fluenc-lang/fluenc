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

		std::string name() const override
		{
			return "...";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt8PtrTy(context);
		}

		bool is(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(type);
			UNUSED(entryPoint);

			return true;
		}

		bool equals(const Type *type, const EntryPoint &entryPoint) const override
		{
			if (dynamic_cast<const IteratorType *>(type))
			{
				return type == this;
			}

			return type->equals(this, entryPoint);
		}
};

#endif // ITERATORTYPE_H
