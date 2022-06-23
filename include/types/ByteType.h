#ifndef BYTETYPE_H
#define BYTETYPE_H

#include "IntegerType.h"

class ByteType : public IntegerType<ByteType>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BYTETYPE_H
