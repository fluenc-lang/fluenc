#include "DzTypeName.h"
#include "EntryPoint.h"
#include "TypedValue.h"

DzTypeName::DzTypeName(const std::string &name)
	: m_name(name)
{
}

std::string DzTypeName::name() const
{
	return m_name;
}

Type *DzTypeName::resolve(const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();

	if (m_name == "int")
	{
		return Int32Type::instance();
	}

	if (m_name == "long")
	{
		return Int64Type::instance();
	}

	if (m_name == "bool")
	{
		return BooleanType::instance();
	}

	if (m_name == "string")
	{
		return StringType::instance();
	}

	auto types = entryPoint.types();

	auto iterator = types.find(m_name);

	if (iterator == types.end())
	{
		throw new std::exception(); // TODO
	}

	return iterator->second;
}

DzTypeName *DzTypeName::int32()
{
	static DzTypeName typeName("int");

	return &typeName;
}

DzTypeName *DzTypeName::int64()
{
	static DzTypeName typeName("long");

	return &typeName;
}

DzTypeName *DzTypeName::boolean()
{
	static DzTypeName typeName("bool");

	return &typeName;
}

DzTypeName *DzTypeName::string()
{
	static DzTypeName typeName("string");

	return &typeName;
}
