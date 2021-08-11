#include "Utility.h"

std::ostream& operator<<(std::ostream &stream, const llvm::StringRef &string)
{
	stream << string.str();

	return stream;
}
