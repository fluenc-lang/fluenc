#ifndef PROXYTYPE_H
#define PROXYTYPE_H

#include "Type.h"

class ProxyType : public Type
{
	public:
		ProxyType(llvm::Type *type);

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;
		bool equals(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		llvm::Type *m_type;
};

#endif // PROXYTYPE_H
