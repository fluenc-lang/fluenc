#ifndef BUFFERTYPE_H
#define BUFFERTYPE_H

#include "BuiltinType.h"

class buffer_type : public BuiltinType<buffer_type>
{
	public:
        type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // BUFFERTYPE_H
