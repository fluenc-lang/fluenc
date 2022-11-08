#include <sstream>

#include <fmt/core.h>

#include "Use.h"

#include "exceptions/FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(Use *use)
	: CompilerException(use->ast())
	, m_fileName(use->fileName())
{
}

std::string FileNotFoundException::message() const
{
	return fmt::format("File '{}' not found in the include path", m_fileName);
}
