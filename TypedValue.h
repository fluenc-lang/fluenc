#ifndef TYPEDVALUE_H
#define TYPEDVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class Type
{
	public:
		virtual std::string tag() const = 0;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;
};

template<typename T>
class BuiltinType : public Type
{
	public:
		static Type *instance()
		{
			static T instance;

			return &instance;
		}
};

class Int32Type : public BuiltinType<Int32Type>
{
	public:
		std::string tag() const override
		{
			return "int";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt32Ty(context);
		}
};

class Int64Type : public BuiltinType<Int64Type>
{
	public:
		std::string tag() const override
		{
			return "long";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt64Ty(context);
		}
};

class BooleanType : public BuiltinType<BooleanType>
{
	public:
		std::string tag() const override
		{
			return "bool";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt1Ty(context);
		}
};

class StringType : public BuiltinType<StringType>
{
	public:
		std::string tag() const override
		{
			return "string";
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt8PtrTy(context);
		}
};

class UserTypeField
{
	public:
		UserTypeField(const std::string &name, Type *type)
			: m_name(name)
			, m_type(type)
		{
		}

		std::string name() const
		{
			return m_name;
		}

		Type *type() const
		{
			return m_type;
		}

	private:
		std::string m_name;

		Type *m_type;
};

class TypedValue
{
	public:
		TypedValue();
		TypedValue(Type *type, llvm::Value *value);

		Type *type() const;

		operator llvm::Value *() const;

	private:
		Type *m_type;

		llvm::Value *m_value;
};

class DzValue;

class PrototypeField
{
	public:
		PrototypeField()
			: m_defaultValue(nullptr)
		{
		}

		PrototypeField(const std::string &name
			, DzValue *defaultValue
			)
			: m_name(name)
			, m_defaultValue(defaultValue)
		{
		}

		std::string name() const
		{
			return m_name;
		}

		DzValue *defaultValue() const
		{
			return m_defaultValue;
		}

	private:
		std::string m_name;

		DzValue *m_defaultValue;
};

class Prototype : public Type
{
	public:
		Prototype(const std::string &tag, const std::vector<PrototypeField> &fields)
			: m_tag(tag)
			, m_fields(fields)
		{
		}

		std::string tag() const override
		{
			return m_tag;
		}

		std::vector<PrototypeField> fields() const
		{
			return m_fields;
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt8PtrTy(context);
		}

	private:
		std::string m_tag;
		std::vector<PrototypeField> m_fields;
};

class UserType : public Type
{
	public:
		UserType(Prototype *prototype, llvm::Type *type, std::vector<UserTypeField *> fields)
			: m_prototype(prototype)
			, m_type(type)
			, m_fields(fields)
		{
		}

		std::string tag() const override
		{
			return m_prototype->tag();
		}

		llvm::Type *storageType(llvm::LLVMContext &) const override
		{
			return m_type;
		}

		std::vector<UserTypeField *> fields() const
		{
			return m_fields;
		}

	private:
		Prototype *m_prototype;

		llvm::Type *m_type;

		std::vector<UserTypeField *> m_fields;
};

#endif // TYPEDVALUE_H
