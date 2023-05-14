#include "DzTypeName.h"
#include "EntryPoint.h"

#include "types/Int32Type.h"
#include "types/Int64Type.h"
#include "types/Uint32Type.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/VoidType.h"
#include "types/WithoutType.h"
#include "types/Prototype.h"
#include "types/ByteType.h"
#include "types/AnyType.h"
#include "types/Float32Type.h"
#include "types/IteratorType.h"
#include "types/BufferType.h"

#include "exceptions/UnknownTypeException.h"

DzTypeName::DzTypeName(const std::shared_ptr<peg::Ast> &ast, const std::vector<std::string> &names)
	: m_ast(ast)
	, m_names(names)
{
}

Type *DzTypeName::resolve(const EntryPoint &entryPoint) const
{
	for (auto &name : m_names)
	{
		if (name == "i32")
		{
			return Int32Type::instance();
		}

		if (name == "i64")
		{
			return Int64Type::instance();
		}

		if (name == "u32")
		{
			return Uint32Type::instance();
		}

		if (name == "u8")
		{
			return ByteType::instance();
		}

		if (name == "f32")
		{
			return Float32Type::instance();
		}

		if (name == "bool")
		{
			return BooleanType::instance();
		}

		if (name == "string")
		{
			return StringType::instance();
		}

		if (name == "void")
		{
			return VoidType::instance();
		}

		if (name == "without")
		{
			return WithoutType::instance();
		}

		if (name == "any")
		{
			return AnyType::instance();
		}

		if (name == "buffer")
		{
			return BufferType::instance();
		}

		if (name == "...")
		{
			return IteratorType::instance();
		}

		auto &types = entryPoint.types();

		auto iterator = types.find(name);

		if (iterator != types.end())
		{
			return iterator->second;
		}
	}

	throw UnknownTypeException(m_ast, m_names[0]);
}

DzTypeName *DzTypeName::int32()
{
	static DzTypeName typeName(nullptr, { "i32" });

	return &typeName;
}

DzTypeName *DzTypeName::int64()
{
	static DzTypeName typeName(nullptr, { "i64" });

	return &typeName;
}

DzTypeName *DzTypeName::f32()
{
	static DzTypeName typeName(nullptr, { "f32" });

	return &typeName;
}

DzTypeName *DzTypeName::uint32()
{
	static DzTypeName typeName(nullptr, { "u32" });

	return &typeName;
}

DzTypeName *DzTypeName::byte()
{
	static DzTypeName typeName(nullptr, { "u8" });

	return &typeName;
}

DzTypeName *DzTypeName::boolean()
{
	static DzTypeName typeName(nullptr, { "bool" });

	return &typeName;
}

DzTypeName *DzTypeName::string()
{
	static DzTypeName typeName(nullptr, { "string" });

	return &typeName;
}

DzTypeName *DzTypeName::without()
{
	static DzTypeName typeName(nullptr, { "without" });

	return &typeName;
}
