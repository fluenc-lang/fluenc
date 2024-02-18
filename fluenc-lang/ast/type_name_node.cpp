#include "type_name_node.hpp"

type_name_t default_type_name::int32()
{
	static default_type_name instance { .names = { "i32" } };

	return &instance;
}

type_name_t default_type_name::int64()
{
	static default_type_name instance { .names = { "i64" } };

	return &instance;
}

type_name_t default_type_name::f32()
{
	static default_type_name instance { .names = { "f32" } };

	return &instance;
}

type_name_t default_type_name::uint32()
{
	static default_type_name instance { .names = { "u32" } };

	return &instance;
}

type_name_t default_type_name::byte()
{
	static default_type_name instance { .names = { "u8" } };

	return &instance;
}

type_name_t default_type_name::boolean()
{
	static default_type_name instance { .names = { "bool" } };

	return &instance;
}

type_name_t default_type_name::string()
{
	static default_type_name instance { .names = { "string" } };

	return &instance;
}

type_name_t default_type_name::without()
{
	static default_type_name instance { .names = { "without" } };

	return &instance;
}
