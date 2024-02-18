#include <numeric>
#include <sstream>
#include <unordered_map>

#include "types/aggregate_type.hpp"

namespace fluenc::code_generation::types
{
	aggregate_type::aggregate_type(const std::vector<const base_type*>& subjects)
		: subjects_(subjects)
	{
	}

	type_id aggregate_type::id() const
	{
		return type_id::aggregate;
	}

	std::string aggregate_type::name() const
	{
		std::ostringstream stream;

		stream << "<";

		for (auto i = begin(subjects_); i != end(subjects_); i++)
		{
			if (i != begin(subjects_))
			{
				stream << ", ";
			}

			stream << (*i)->name();
		}

		stream << ">";

		return stream.str();
	}

	std::string aggregate_type::full_name() const
	{
		std::ostringstream stream;

		stream << "<";

		for (auto i = begin(subjects_); i != end(subjects_); i++)
		{
			if (i != begin(subjects_))
			{
				stream << ", ";
			}

			stream << (*i)->full_name();
		}

		stream << ">";

		return stream.str();
	}

	std::vector<const base_type*> aggregate_type::subjects() const
	{
		return subjects_;
	}

	llvm::Type* aggregate_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt1Ty(context);
	}

	aggregate_type* aggregate_type::get(const std::vector<const base_type*>& types)
	{
		static std::unordered_map<size_t, aggregate_type> cache;

		auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type) {
			return (accumulator ^ reinterpret_cast<size_t>(type)) * 1099511628211;
		});

		auto [iterator, _] = cache.try_emplace(hash, types);

		return &iterator->second;
	}
}
