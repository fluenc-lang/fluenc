#ifndef OPAQUEPOINTERTYPE_H
#define OPAQUEPOINTERTYPE_H

#include "Type.h"

class IPrototype;

class OpaquePointerType : public Type
{
	public:
		OpaquePointerType(const IPrototype *subject);

		TypeId id() const override;

		std::string name() const override;
		std::string fullName() const override;

		const IPrototype *subject() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static OpaquePointerType *get(const IPrototype *subject);

	private:
		const IPrototype *m_subject;
};

#endif // OPAQUEPOINTERTYPE_H
