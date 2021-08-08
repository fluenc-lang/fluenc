#ifndef DZFUNCTION_H
#define DZFUNCTION_H

#include "DzCallable.h"

class DzFunction : public DzCallable
{
	public:
		DzFunction(const std::string &name
			, std::vector<std::string> arguments
			, DzValue *block
			);

		std::string name() const override;

		FunctionAttribute attribute() const override;

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		std::string m_name;
		std::vector<std::string> m_arguments;

		DzValue *m_block;
};


#endif // DZFUNCTION_H
