#ifndef DZFIELDACCESS_H
#define DZFIELDACCESS_H

#include "DzValue.h"

#include "values/NamedValue.h"

class DzFieldAccess : public DzValue
{
	public:
		DzFieldAccess(llvm::Value *instance, const NamedValue *field, size_t index);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::Value *m_instance;

		const NamedValue *m_field;

		size_t m_index;
};

#endif // DZFIELDACCESS_H
