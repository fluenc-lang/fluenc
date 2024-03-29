#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include "IPrototype.h"
#include "PrototypeFieldEmbryo.h"

class ITypeName;

class Prototype : public IPrototype
{
	public:
		Prototype(const std::shared_ptr<peg::Ast> &ast
			, const std::string &tag
			, const std::vector<PrototypeFieldEmbryo> &fields
			, const std::vector<ITypeName *> &parentTypes
			);

		TypeId id() const override
		{
			return TypeId::Prototype;
		}

		const std::shared_ptr<peg::Ast> ast() const;

		const IPrototype *root() const override;

		std::string name() const override;
		std::pair<EntryPoint, std::vector<PrototypeField>> fields(const EntryPoint &entryPoint, const DefaultNodeVisitor &visitor) const override;
		std::vector<ITypeName *> parentTypes() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

	private:
		const std::shared_ptr<peg::Ast> m_ast;

		std::pair<bool, int8_t> compatibility(int8_t score, const Type *type, const EntryPoint &entryPoint) const;

		std::string m_tag;

		std::vector<PrototypeFieldEmbryo> m_fields;
		std::vector<ITypeName *> m_parentTypes;
};

#endif // PROTOTYPE_H
