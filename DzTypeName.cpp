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

#include "exceptions/UnknownTypeException.h"

DzTypeName::DzTypeName(antlr4::ParserRuleContext *context, const std::string &name)
	: m_context(context)
	, m_name(name)
{
}

Type *DzTypeName::resolve(const EntryPoint &entryPoint) const
{
	if (m_name == "int")
	{
		return Int32Type::instance();
	}

	if (m_name == "long")
	{
		return Int64Type::instance();
	}

	if (m_name == "uint")
	{
		return Uint32Type::instance();
	}

	if (m_name == "byte")
	{
		return ByteType::instance();
	}

	if (m_name == "bool")
	{
		return BooleanType::instance();
	}

	if (m_name == "string")
	{
		return StringType::instance();
	}

	if (m_name == "void")
	{
		return VoidType::instance();
	}

	if (m_name == "without")
	{
		return WithoutType::instance();
	}

	if (m_name == "any")
	{
		return AnyType::instance();
	}

	if (m_name == "...")
	{
		return IteratorType::instance();
	}

	auto types = entryPoint.types();

	auto iterator = types.find(m_name);

	if (iterator == types.end())
	{
		throw new UnknownTypeException(m_context, m_name);
	}

	return iterator->second;
}

DzTypeName *DzTypeName::int32()
{
	static DzTypeName typeName(nullptr, "int");

	return &typeName;
}

DzTypeName *DzTypeName::int64()
{
	static DzTypeName typeName(nullptr, "long");

	return &typeName;
}

DzTypeName *DzTypeName::uint32()
{
	static DzTypeName typeName(nullptr, "uint");

	return &typeName;
}

DzTypeName *DzTypeName::byte()
{
	static DzTypeName typeName(nullptr, "byte");

	return &typeName;
}

DzTypeName *DzTypeName::boolean()
{
	static DzTypeName typeName(nullptr, "bool");

	return &typeName;
}

DzTypeName *DzTypeName::string()
{
	static DzTypeName typeName(nullptr, "string");

	return &typeName;
}

DzTypeName *DzTypeName::without()
{
	static DzTypeName typeName(nullptr, "without");

	return &typeName;
}
