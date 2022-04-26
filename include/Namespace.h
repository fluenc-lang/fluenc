#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <any>

#include "Node.h"

class Namespace
{
	public:
		Namespace(const std::vector<std::any > &children
			, const std::string &name
			);

		std::vector<std::any> children() const;
		std::string name() const;

	private:
		std::vector<std::any> m_children;
		std::string m_name;
};

#endif // NAMESPACE_H
