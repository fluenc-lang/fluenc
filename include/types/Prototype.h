#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include "IPrototype.h"
#include "PrototypeFieldEmbryo.h"

class ITypeName;

class Prototype : public IPrototype
{
	public:
		Prototype(const std::string &tag
			, const std::vector<PrototypeFieldEmbryo> &fields
			, const std::vector<ITypeName *> &parentTypes
			);

		const IPrototype *root() const override;

		std::string name() const override;
		std::vector<PrototypeField> fields(const EntryPoint &entryPoint) const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		std::pair<bool, int8_t> compatibility(int8_t score, const Type *type, const EntryPoint &entryPoint) const;

		std::string m_tag;

		std::vector<PrototypeFieldEmbryo> m_fields;
		std::vector<ITypeName *> m_parentTypes;
};

#endif // PROTOTYPE_H
