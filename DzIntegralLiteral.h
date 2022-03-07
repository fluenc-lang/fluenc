#ifndef DZINTEGRALLITERAL_H
#define DZINTEGRALLITERAL_H

#include <llvm/IR/Constants.h>

#include "DzValue.h"

class ITypeName;

class DzIntegralLiteral : public DzValue
{
	public:
		DzIntegralLiteral(DzValue *consumer, ITypeName *type, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		DzValue *m_consumer;
		ITypeName *m_type;

		std::string m_value;
};

#endif // DZINTEGRALLITERAL_H
