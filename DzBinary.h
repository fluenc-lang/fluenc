#ifndef DZBINARY_H
#define DZBINARY_H

#include "antlr4-runtime/dzParser.h"

#include "DzValue.h"

class DzBinary : public DzValue
{
	public:
		DzBinary();

		DzValue *left() const;
		void setLeft(DzValue *left);

		DzValue *right() const;
		void setRight(DzValue *value);

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		DzValue *m_left;
		DzValue *m_right;
};

#endif // DZBINARY_H
