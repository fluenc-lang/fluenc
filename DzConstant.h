#ifndef DZCONSTANT_H
#define DZCONSTANT_H

#include "DzValue.h"

class DzConstant : public DzValue
{
	public:
		DzConstant(DzValue *consumer, std::string value);

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZCONSTANT_H
