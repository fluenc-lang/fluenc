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

DzTypeName::DzTypeName(antlr4::ParserRuleContext *context, const std::vector<std::string> &names)
	: m_token(TokenInfo::fromContext(context))
	, m_names(names)
{
}

Type *DzTypeName::resolve(const EntryPoint &entryPoint) const
{
	for (auto &name : m_names)
	{
		if (name == "int")
		{
			return Int32Type::instance();
		}

		if (name == "long")
		{
			return Int64Type::instance();
		}

		if (name == "uint")
		{
			return Uint32Type::instance();
		}

		if (name == "byte")
		{
			return ByteType::instance();
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

		if (name == "...")
		{
			return IteratorType::instance();
		}

		auto types = entryPoint.types();

		auto iterator = types.find(name);

		if (iterator != types.end())
		{
			return iterator->second;
		}
	}

	throw new UnknownTypeException(m_token, m_names[0]);
}

DzTypeName *DzTypeName::int32()
{
	static DzTypeName typeName(nullptr, { "int" });

	return &typeName;
}

DzTypeName *DzTypeName::int64()
{
	static DzTypeName typeName(nullptr, { "long" });

	return &typeName;
}

DzTypeName *DzTypeName::uint32()
{
	static DzTypeName typeName(nullptr, { "uint" });

	return &typeName;
}

DzTypeName *DzTypeName::byte()
{
	static DzTypeName typeName(nullptr, { "byte" });

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
