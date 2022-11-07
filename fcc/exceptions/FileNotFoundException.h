#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <string>

#include "CompilerException.h"

class Use;

class FileNotFoundException : public CompilerException
{
	public:
		FileNotFoundException(Use *use);

		std::string message() const;

	private:
		std::string m_fileName;
};

#endif // FILENOTFOUNDEXCEPTION_H
