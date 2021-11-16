#ifndef DZARGUMENT_H
#define DZARGUMENT_H

#include <string>

#include <llvm/IR/Type.h>

#include "DzBaseArgument.h"

#include "values/TupleValue.h"

class DzTypeName;
class EntryPoint;
class Type;

class DzArgument : public DzBaseArgument
{
	public:
		DzArgument(const std::string &name, DzTypeName *type);

		std::string name() const;

		Type *type(const EntryPoint &entryPoint) const override;

	private:
		std::string m_name;

		DzTypeName *m_type;
};

class DzTupleArgument : public DzBaseArgument
{
	public:
		DzTupleArgument(const std::vector<DzBaseArgument *> &arguments)
			: m_arguments(arguments)
		{
		}

		std::vector<DzBaseArgument *> arguments() const
		{
			return m_arguments;
		}

		Type *type(const EntryPoint &entryPoint) const override
		{
			std::vector<const Type *> types;

			std::transform(rbegin(m_arguments), rend(m_arguments), std::back_insert_iterator(types), [&](auto arguments)
			{
				return arguments->type(entryPoint);
			});

			return TupleType::get(types);
		}

	private:
		std::vector<DzBaseArgument *> m_arguments;
};

#endif // DZARGUMENT_H
