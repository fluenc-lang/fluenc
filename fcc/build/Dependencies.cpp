#include <filesystem>

#include <fmt/core.h>

#include <git2.h>

#include "Dependencies.h"
#include "Build.h"

bool ensureDirectory(const std::filesystem::path &path)
{
	std::error_code error;

	if (create_directories(path, error))
	{
		return true;
	}

	if (error)
	{
		fmt::print("Failed to create directory {}: {}\n"
			, path.string()
			, error.message()
			);

		return false;
	}

	return true;
}

bool buildDependencies(const BuildConfiguration &config, const BuildContext &context)
{
	auto repos_dir = context.environment.root / ".fluenc" / "repos";

	if (!ensureDirectory(repos_dir))
	{
		return false;
	}

	auto modules_dir = context.environment.root / ".fluenc" / "modules";

	if (!ensureDirectory(modules_dir))
	{
		return false;
	}

	for (auto &[name, url] : config.repos)
	{
		git_repository *repo = nullptr;

		auto repo_dir = absolute(repos_dir / name);

		if (!exists(repo_dir))
		{
			fmt::print("Fetching repository '{}' to {}...\n", name, repo_dir.string());

			if (git_clone(&repo, url.c_str(), repo_dir.c_str(), nullptr) < 0)
			{
				auto error = git_error_last();

				fmt::print("Failed to clone repo '{}' ({}): {}\n", name, url, error->message);

				return false;
			}
		}
		else
		{
			fmt::print("Updating repository '{}'...\n", name);

			if (git_repository_open(&repo, repo_dir.c_str()) < 0)
			{
				auto error = git_error_last();

				fmt::print("Failed to open repo '{}' ({}): {}\n", name, repo_dir.string(), error->message);

				return false;
			}

			git_remote *remote = nullptr;

			if (git_remote_lookup(&remote, repo, "origin") < 0)
			{
				auto error = git_error_last();

				fmt::print("Failed to find remote 'origin' for repo '{}' ({}): {}\n", name, repo_dir.string(), error->message);

				return false;
			}

			if (git_remote_fetch(remote, nullptr, nullptr, nullptr) < 0)
			{
				auto error = git_error_last();

				fmt::print("Failed to fetch repo '{}' ({}): {}\n", name, repo_dir.string(), error->message);

				return false;
			}

			auto reset_to_revision = [](const char *, const char *, const git_oid *oid, unsigned int , void *payload)
			{
				auto repo = reinterpret_cast<git_repository *>(payload);

				git_object *object = nullptr;

				if (auto result = git_object_lookup(&object, repo, oid, GIT_OBJECT_COMMIT); result < 0)
				{
					return result;
				}

				if (auto result = git_reset(repo, object, GIT_RESET_HARD, nullptr); result < 0)
				{
					return result;
				}

				return 0;
			};

			if (git_repository_fetchhead_foreach(repo, reset_to_revision, repo) < 0)
			{
				auto error = git_error_last();

				fmt::print("Failed to reset to FETCH_HEAD for repo '{}' ({}): {}\n", name, repo_dir.string(), error->message);

				return false;
			}
		}
	}

	for (auto &name : config.modules)
	{
		auto delimiter = name.find("/");

		if (delimiter == std::string::npos)
		{
			fmt::print("Invalid module name '{}'\n", name);

			return false;
		}

		std::string repo_name(begin(name), begin(name) + delimiter);
		std::string module_name(begin(name) + delimiter + 1, end(name));

		auto repo_dir = repos_dir / repo_name;

		if (!exists(repo_dir))
		{
			fmt::print("Repo '{}' does not exist\n", repo_name);

			return false;
		}

		auto module_source_dir = repo_dir / module_name;

		if (!exists(module_source_dir))
		{
			fmt::print("Module '{}' does not exist in repo {}\n", module_name, repo_name);

			return false;
		}

		auto module_target_dir = modules_dir / repo_name;

		if (!ensureDirectory(module_target_dir))
		{
			return false;
		}

		BuildContext dependencyContext = {
			.options = context.options,
			.environment = {
				.source = module_source_dir,
				.target = module_target_dir,
				.root = context.environment.root
			},
			.clangDriver = context.clangDriver,
			.targetMachine = context.targetMachine,
			.llvmContext = context.llvmContext,
		};

		if (!build(dependencyContext))
		{
			return false;
		}
	}

	return true;
}
