#ifndef DZTERMINATOR_H
#define DZTERMINATOR_H

#include "antlr4-runtime/dzParser.h"

#include "DzValue.h"

class DzTerminator : public DzValue
{
	public:
		DzTerminator();

		DzValue *value() const;
		void setValue(DzValue *value);

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		DzValue *m_value;
};
#endif // DZTERMINATOR_H
