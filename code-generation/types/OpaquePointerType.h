#ifndef OPAQUEPOINTERTYPE_H
#define OPAQUEPOINTERTYPE_H

#include "Type.h"

class IPrototype;

class structure_type;

class opaque_pointer_type : public Type
{
	public:
        opaque_pointer_type(const structure_type *subject);

        type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		const structure_type *subject() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

                static opaque_pointer_type *get(const structure_type *subject);

	private:
		const structure_type *m_subject;
};

#endif // OPAQUEPOINTERTYPE_H
