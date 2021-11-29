#include <sstream>

#include <llvm/IR/IRBuilder.h>

#include "Utility.h"

std::ostream& operator<<(std::ostream &stream, const llvm::StringRef &string)
{
	stream << string.str();

	return stream;
}

void linkBlocks(llvm::BasicBlock *source, llvm::BasicBlock *target)
{
	llvm::IRBuilder<> builder(source);
	builder.CreateBr(target);
}

std::string nextTag()
{
	static int i;

	std::stringstream ss;
	ss << "tag";
	ss << i++;

	return ss.str();
}
