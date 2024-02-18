#ifndef BYTETYPE_H
#define BYTETYPE_H

#include "IntegerType.h"

class byte_type : public IntegerType<byte_type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BYTETYPE_H
