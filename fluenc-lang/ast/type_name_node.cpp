#include "type_name_node.hpp"

type_name_t default_type_name::int32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "i32" },
	};

	return &instance;
}

type_name_t default_type_name::int64()
{
	static default_type_name instance {
		.ast = {},
		.names = { "i64" },
	};

	return &instance;
}

type_name_t default_type_name::f32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "f32" },
	};

	return &instance;
}

type_name_t default_type_name::uint32()
{
	static default_type_name instance {
		.ast = {},
		.names = { "u32" },
	};

	return &instance;
}

type_name_t default_type_name::byte()
{
	static default_type_name instance {
		.ast = {},
		.names = { "u8" },
	};

	return &instance;
}

type_name_t default_type_name::boolean()
{
	static default_type_name instance {
		.ast = {},
		.names = { "bool" },
	};

	return &instance;
}

type_name_t default_type_name::string()
{
	static default_type_name instance {
		.ast = {},
		.names = { "string" },
	};

	return &instance;
}

type_name_t default_type_name::without()
{
	static default_type_name instance {
		.ast = {},
		.names = { "without" },
	};

	return &instance;
}
