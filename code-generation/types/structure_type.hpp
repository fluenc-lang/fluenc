#pragma once

#include <string>

#include "Type.h"

struct structure_field
{
	std::string name;

	const Type* type;
};

class type_provider;

namespace fluenc
{
    struct struct_node;
}

class structure_type : public Type
{
	public:
        structure_type(const fluenc::struct_node *subject);

        type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		std::vector<structure_field> fields(const EntryPoint &type_resolver) const;

                const fluenc::struct_node *subject() const;

                static structure_type* get(const fluenc::struct_node* subject);

	private:
                const fluenc::struct_node* m_subject;
};
