#ifndef DZFIELDACCESS_H
#define DZFIELDACCESS_H

#include "DzValue.h"

#include "types/UserTypeField.h"

class DzFieldAccess : public DzValue
{
	public:
		DzFieldAccess(llvm::Value *instance, const UserTypeField &field);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::Value *m_instance;

		UserTypeField m_field;
};

#endif // DZFIELDACCESS_H
