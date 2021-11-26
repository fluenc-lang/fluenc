#ifndef WITHPROTOTYPE_H
#define WITHPROTOTYPE_H

#include "IPrototype.h"
#include "DzValue.h"

#include "values/UserTypeValue.h"

class TypedValue;

class WithPrototype : public IPrototype
{
	public:
		WithPrototype(const UserTypeValue *value);

		std::string tag() const override;
		std::vector<const NamedValue *> fields(const EntryPoint &entryPoint) const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		const UserTypeValue *m_value;
};

#endif // WITHPROTOTYPE_H
