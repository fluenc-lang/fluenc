#ifndef BUFFERTYPE_H
#define BUFFERTYPE_H

#include "BuiltinType.h"

class BufferType : public BuiltinType<BufferType>
{
	public:
		TypeId id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BUFFERTYPE_H
