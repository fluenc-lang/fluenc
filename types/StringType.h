#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include "BuiltinType.h"

class StringType : public BuiltinType<StringType>
{
	public:
		std::string tag() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};

#endif // STRINGTYPE_H
