#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include "IPrototype.h"
#include "PrototypeFieldEmbryo.h"

class DzTypeName;

class Prototype : public IPrototype
{
	public:
		Prototype(const std::string &tag
			, const std::vector<PrototypeFieldEmbryo> &fields
			, const std::vector<DzTypeName *> &parentTypes
			);

		std::string name() const override;
		std::vector<PrototypeField> fields(const EntryPoint &entryPoint) const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		std::string m_tag;

		std::vector<PrototypeFieldEmbryo> m_fields;
		std::vector<DzTypeName *> m_parentTypes;
};

#endif // PROTOTYPE_H
