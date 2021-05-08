#ifndef DZFUNCTION_H
#define DZFUNCTION_H

#include <sstream>

#include "DzValue.h"
#include "FunctionAttribute.h"

class EntryPoint;
class DzTypeName;
class DzMember;

class DzFunction : public DzValue
{
	public:
		DzFunction(const std::string &name
			, const std::vector<DzMember *> &arguments
			, FunctionAttribute attribute
			, DzTypeName *returnType
			, DzValue *block
			);

		std::string name() const;
		std::vector<DzMember *> arguments() const;

		FunctionAttribute attribute() const;

		DzTypeName *returnType() const;

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		std::string m_name;
		std::vector<DzMember *> m_arguments;

		FunctionAttribute m_attribute;

		DzTypeName *m_returnType;
		DzValue *m_block;
};

#endif // DZFUNCTION_H
