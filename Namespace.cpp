#include "Namespace.h"

Namespace::Namespace(const std::vector<antlrcpp::Any> &children, const std::string &name)
	: m_children(children)
	, m_name(name)
{
}

std::vector<antlrcpp::Any> Namespace::children() const
{
	return m_children;
}

std::string Namespace::name() const
{
	return m_name;
}
