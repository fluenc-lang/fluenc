#ifndef USERTYPE_H
#define USERTYPE_H

#include "Type.h"

class IPrototype;

class UserType : public Type
{
	public:
		UserType(const IPrototype *prototype, const std::vector<const Type *> &elementTypes);

		TypeId id() const override;

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		const IPrototype *prototype() const;

		IOperatorSet *operators() const override;

		std::vector<const Type *> elementTypes() const;

		static UserType *get(const IPrototype *prototype, const std::vector<const Type *> &elementTypes);

	private:
		const IPrototype *m_prototype;

		std::vector<const Type *> m_elementTypes;
};

#endif // USERTYPE_H
