#ifndef DZTYPENAME_H
#define DZTYPENAME_H

#include <vector>
#include <memory>

#include "ITypeName.h"
#include "TokenInfo.h"

class EntryPoint;
class Type;

class DzTypeName : public ITypeName
{
	public:
		DzTypeName(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string> &names);

		Type *resolve(const EntryPoint &entryPoint) const override;

		static DzTypeName *int32();
		static DzTypeName *int64();
		static DzTypeName *uint32();
		static DzTypeName *byte();
		static DzTypeName *boolean();
		static DzTypeName *string();
		static DzTypeName *without();

	private:
		std::shared_ptr<peg::Ast> m_ast;
		std::vector<std::string> m_names;
};

#endif // DZTYPENAME_H
