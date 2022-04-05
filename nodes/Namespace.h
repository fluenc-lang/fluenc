#ifndef NAMESPACE_H
#define NAMESPACE_H

#include "antlr4-runtime/support/Any.h"

#include "Node.h"

class Namespace
{
	public:
		Namespace(const std::vector<antlrcpp::Any > &children
			, const std::string &name
			);

		std::vector<antlrcpp::Any> children() const;
		std::string name() const;

	private:
		std::vector<antlrcpp::Any> m_children;
		std::string m_name;
};

#endif // NAMESPACE_H
