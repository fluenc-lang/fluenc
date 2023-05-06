#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include "BuiltinType.h"

class StringType : public BuiltinType<StringType>
{
	public:
		StringType();
		StringType(size_t length);

		TypeId id() const override;

		size_t length() const;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;

	private:
		size_t m_length;
};

#endif // STRINGTYPE_H
