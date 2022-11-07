#include <sstream>

#include "Use.h"

#include "exceptions/FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(Use *use)
	: CompilerException(use->ast())
	, m_fileName(use->fileName())
{
}

std::string FileNotFoundException::message() const
{
	std::ostringstream stream;
	stream << "File '";
	stream << m_fileName;
	stream << "' not found in the include path";

	return stream.str();
}
