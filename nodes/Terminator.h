#ifndef TERMINATOR_H
#define TERMINATOR_H

#include "Node.h"
#include "FunctionAttribute.h"

class Terminator : public Node
{
	public:
		Terminator(const std::string &name, FunctionAttribute attribute);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

		FunctionAttribute attribute() const;

		static Terminator *instance();

	private:
		std::string m_name;

		FunctionAttribute m_attribute;
};

#endif // TERMINATOR_H
