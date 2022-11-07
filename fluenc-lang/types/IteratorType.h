#ifndef ITERATORTYPE_H
#define ITERATORTYPE_H

#include <sstream>

#include "Type.h"
#include "Utility.h"
#include "TupleType.h"

class IteratorType : public Type
{
	public:
		static Type *instance();

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;
};

#endif // ITERATORTYPE_H
