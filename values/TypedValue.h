#ifndef TYPEDVALUE_H
#define TYPEDVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;
class DzValue;

class Blast : public BaseValue
{
	public:
		Blast(const DzValue *subject, const EntryPoint *entryPoint)
			: m_subject(subject)
			, m_entryPoint(entryPoint)
		{
		}

		const DzValue *subject() const
		{
			return m_subject;
		}

		const EntryPoint *entryPoint() const
		{
			return m_entryPoint;
		}

		const Type *type() const override
		{
			return nullptr;
		}

	private:
		const DzValue *m_subject;
		const EntryPoint *m_entryPoint;
};

class TypedValue : public BaseValue
{
	public:
		TypedValue(const Type *type, llvm::Value *value);

		const Type *type() const;

		llvm::Constant *constant() const;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // TYPEDVALUE_H
