#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include "Type.h"
#include "PrototypeField.h"

class Prototype : public Type
{
	public:
		Prototype(const std::string &tag, const std::vector<PrototypeField> &fields);

		std::string tag() const override;
		std::vector<PrototypeField> fields() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

	private:
		std::string m_tag;
		std::vector<PrototypeField> m_fields;
};

#endif // PROTOTYPE_H
