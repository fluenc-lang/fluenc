#include "type_name_node.hpp"

using namespace std::literals::string_literals;

type_name_t default_type_name::int32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "i32"s },
	};

	return &instance;
}

type_name_t default_type_name::int64()
{
	static default_type_name instance {
		.ast = {},
		.names = { "i64"s },
	};

	return &instance;
}

type_name_t default_type_name::f32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "f32"s },
	};

	return &instance;
}

type_name_t default_type_name::uint32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "u32"s },
	};

	return &instance;
}

type_name_t default_type_name::byte()
{
	static default_type_name instance {
		.ast = {},
		.names = { "u8"s },
	};

	return &instance;
}

type_name_t default_type_name::boolean()
{
	static default_type_name instance {
		.ast = {},
		.names = { "bool"s },
	};

	return &instance;
}

type_name_t default_type_name::string()
{
	static default_type_name instance {
		.ast = {},
		.names = { "string"s },
	};

	return &instance;
}

type_name_t default_type_name::without()
{
	static default_type_name instance {
		.ast = {},
		.names = { "without"s },
	};

	return &instance;
}
