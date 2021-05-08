#ifndef DZCLOSURE_H
#define DZCLOSURE_H

#include "DzValue.h"

class DzParameter;

class DzClosure : public DzValue
{
	public:
		DzClosure(int id, DzValue *parent);

		DzParameter *parameter();

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		int m_id;

		DzValue *m_parent;
};

#endif // DZCLOSURE_H
