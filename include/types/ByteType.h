#ifndef BYTETYPE_H
#define BYTETYPE_H

#include "BuiltinType.h"

class ByteType : public BuiltinType<ByteType>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BYTETYPE_H
