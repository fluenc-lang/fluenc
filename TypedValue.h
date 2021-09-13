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

class Prototype : public Type
{
	public:
		Prototype(const std::string &tag, const std::vector<std::string> &fields)
			: m_tag(tag)
			, m_fields(fields)
		{
		}

		std::string tag() const override
		{
			return m_tag;
		}

		std::vector<std::string> fields() const
		{
			return m_fields;
		}

		llvm::Type *storageType(llvm::LLVMContext &context) const override
		{
			return llvm::Type::getInt8PtrTy(context);
		}

	private:
		std::string m_tag;
		std::vector<std::string> m_fields;
};

class UserType : public Type
{
	public:
		UserType(Prototype *prototype, llvm::Type *type)
			: m_prototype(prototype)
			, m_type(type)
		{
		}

		std::string tag() const override
		{
			return m_prototype->tag();
		}

		Prototype *prototype() const
		{
			return m_prototype;
		}

		llvm::Type *storageType(llvm::LLVMContext &) const override
		{
			return m_type;
		}

	private:
		Prototype *m_prototype;

		llvm::Type *m_type;
};

//class Type
//{
//	public:
//		Type()
//			: m_type(nullptr)
//		{
//		}

//		Type(llvm::Type *type, const std::string &name)
//			: m_type(type)
//			, m_name(name)
//		{
//		}

//		std::string name() const
//		{
//			return m_name;
//		}

//		operator llvm::Type *() const
//		{
//			return m_type;
//		}

//	private:
//		llvm::Type *m_type;

//		std::string m_name;
//};

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

#endif // TYPEDVALUE_H
