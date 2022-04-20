#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include "BuiltinType.h"

class StringType : public BuiltinType<StringType>
{
	public:
		StringType();
		StringType(size_t length);

		size_t length() const;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static StringType *get(size_t length);

	private:
		size_t m_length;
};

#endif // STRINGTYPE_H
