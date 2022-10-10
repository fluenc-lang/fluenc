#ifndef USE_H
#define USE_H

#include <string>
#include <memory>

#include "TokenInfo.h"

class Use
{
	public:
		Use(const std::shared_ptr<peg::Ast> &ast, const std::string &fileName);

		std::shared_ptr<peg::Ast> ast() const;
		std::string fileName() const;

	private:
		std::shared_ptr<peg::Ast> m_ast;
		std::string m_fileName;
};

#endif // USE_H
