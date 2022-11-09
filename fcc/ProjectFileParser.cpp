#define TOML_EXCEPTIONS 0

#include <numeric>

#include <toml++/toml.h>

#include "ProjectFileParser.h"

std::optional<BuildConfiguration> ProjectFileParser::parse(const std::string &fileName)
{
	auto table = toml::parse_file(fileName);

	if (!table)
	{
		return {};
	}

	return std::accumulate(table.begin(), table.end(), BuildConfiguration(), [](auto accumulation, toml::impl::table_proxy_pair<false> row) -> BuildConfiguration
	{
		auto &[key, value] = row;

		if (key.str() == "type")
		{
			auto type = value.value<std::string>();

			if (!type)
			{
				return accumulation;
			}

			return
			{
				*type,
				accumulation.target,
				accumulation.libs
			};
		}

		if (key.str() == "target")
		{
			auto target = value.value<std::string>();

			if (!target)
			{
				return accumulation;
			}

			return
			{
				accumulation.type,
				*target,
				accumulation.libs
			};
		}

		if (key.str() == "libs")
		{
			if (auto array = value.as_array())
			{
				std::vector<std::string> libs;

				for (auto &element : *array)
				{
					auto lib = element.value<std::string>();

					if (lib)
					{
						libs.push_back(*lib);
					}
				}

				return
				{
					accumulation.type,
					accumulation.target,
					libs
				};
			}

			return accumulation;
		}

		return accumulation;
	});
}
