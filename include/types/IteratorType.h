#ifndef ITERATORTYPE_H
#define ITERATORTYPE_H

#include <sstream>

#include "Type.h"
#include "Utility.h"
#include "TupleType.h"

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

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override
		{
			UNUSED(entryPoint);

			if (auto other = dynamic_cast<const IteratorType *>(type))
			{
				if (other == this)
				{
					return 0;
				}

				return 1;
			}

			return type->compatibility(this, entryPoint);
		}
};

#endif // ITERATORTYPE_H
