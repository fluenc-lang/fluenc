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
				accumulation.libs,
				accumulation.modules,
				accumulation.repos,
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
				accumulation.libs,
				accumulation.modules,
				accumulation.repos,
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
					libs,
					accumulation.modules,
					accumulation.repos,
				};
			}

			return accumulation;
		}

		if (key.str() == "modules")
		{
			if (auto array = value.as_table())
			{
				std::vector<std::string> modules;

				for (auto &[key, value] : *array)
				{
					modules.push_back(std::string(key.str()));
				}

				return
				{
					accumulation.type,
					accumulation.target,
					accumulation.libs,
					modules,
					accumulation.repos,
				};
			}

			return accumulation;
		}

		if (key.str() == "repos")
		{
			if (auto array = value.as_table())
			{
				std::unordered_map<std::string, std::string> repos;

				for (auto &[key, value] : *array)
				{
					if (auto url = value.as_string())
					{
						repos.emplace(std::string(key.str()), *url);
					}
				}

				return
				{
					accumulation.type,
					accumulation.target,
					accumulation.libs,
					accumulation.modules,
					repos,
				};
			}

			return accumulation;
		}

		return accumulation;
	});
}
