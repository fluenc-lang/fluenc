#ifndef DZCHARACTERLITERAL_H
#define DZCHARACTERLITERAL_H

#include <llvm/IR/Constants.h>

#include "DzValue.h"

class DzCharacterLiteral : public DzValue
{
	public:
		DzCharacterLiteral(DzValue *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		DzValue *m_consumer;

		std::string m_value;
};

#endif // DZCHARACTERLITERAL_H
