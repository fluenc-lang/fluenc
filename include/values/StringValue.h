#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include "BaseValue.h"

class ScalarValue;
class LazyValue;

class StringValue : public BaseValue
{
	public:
		StringValue(llvm::Value *address, size_t id, size_t length);

		const ScalarValue *scalar() const;
		const LazyValue *iterator() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

	private:
		llvm::Value *m_address;

		size_t m_id;
		size_t m_length;
};

#endif // STRINGVALUE_H
