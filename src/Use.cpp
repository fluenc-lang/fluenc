#include "Use.h"

Use::Use(const std::string &fileName)
	: m_fileName(fileName)
{
}

std::string Use::fileName() const
{
	return m_fileName.substr(1, m_fileName.size() - 2);
}
