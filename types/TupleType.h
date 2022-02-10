#ifndef TUPLETYPE_H
#define TUPLETYPE_H

#include <vector>

#include "Type.h"

class IteratorType;

class TupleType : public Type
{
	public:
		TupleType(const Type *iteratorType, const std::vector<const Type *> types);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;
		bool equals(const Type *type, const EntryPoint &entryPoint) const override;

		static TupleType *get(const Type *iteratorType, const std::vector<const Type *> &types);

	private:
		const Type *m_iteratorType;

		std::vector<const Type *> m_types;
};

#endif // TUPLETYPE_H
