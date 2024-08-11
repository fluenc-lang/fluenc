#pragma once

#define TOML_EXCEPTIONS 0

#include <filesystem>
#include <fstream>

#include <toml++/toml.h>

int init()
{
	std::ofstream stream("project.toml");

	auto current_path = std::filesystem::current_path();

	auto target_name = current_path.filename().string();

	auto table = toml::table {
		{ "target", target_name },
		{
			"modules",
			toml::table {
				{ "std/array", "1.0" },
				{ "std/string", "1.0" },
			},
		},
		{
			"repos",
			toml::table {
				{ "std", "https://github.com/Znurre/fluenc-modules" },
			},
		},
	};

	stream << table << std::endl;

	return 0;
}
