#ifndef TUPLETYPE_H
#define TUPLETYPE_H

#include <vector>

#include "Type.h"

class TupleType : public Type
{
	public:
		TupleType(const std::vector<const Type *> types);

		std::string tag() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

		static TupleType *get(const std::vector<const Type *> types);

	private:
		std::vector<const Type *> m_types;
};

#endif // TUPLETYPE_H
