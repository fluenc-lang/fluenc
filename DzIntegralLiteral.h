#ifndef DZINTEGRALLITERAL_H
#define DZINTEGRALLITERAL_H

#include <llvm/IR/Constants.h>

#include "DzValue.h"

class DzTypeName;

class DzIntegralLiteral : public DzValue
{
	public:
		DzIntegralLiteral(DzValue *consumer, DzTypeName *type, const std::string &value);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		DzValue *m_consumer;
		DzTypeName *m_type;

		std::string m_value;
};

#endif // DZINTEGRALLITERAL_H
