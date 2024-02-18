#ifndef ITERATORTYPE_H
#define ITERATORTYPE_H

#include <sstream>

#include "TupleType.h"
#include "Type.h"
#include "utility.hpp"

struct iterator;

class iterator_type : public Type
{
	public:
		static Type *instance();

                type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static Type* get(const iterator* it);
};

#endif // ITERATORTYPE_H
