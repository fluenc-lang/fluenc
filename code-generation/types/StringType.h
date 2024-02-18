#pragma once

#include "BuiltinType.h"

class string_type : public BuiltinType<string_type>
{
	public:
		string_type();
		string_type(size_t length);

                type_id id() const override;

		size_t length() const;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;

	private:
		size_t m_length;
};
