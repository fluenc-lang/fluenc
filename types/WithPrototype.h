#ifndef WITHPROTOTYPE_H
#define WITHPROTOTYPE_H

#include "IPrototype.h"
#include "DzValue.h"

class TypedValue;

class WithPrototype : public IPrototype
{
	public:
		WithPrototype(const TypedValue *value);

		std::string tag() const override;
		std::vector<PrototypeField> fields(const EntryPoint &entryPoint) const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		Type *iteratorType() const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		const TypedValue *m_value;
};

#endif // WITHPROTOTYPE_H
