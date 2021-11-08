#ifndef DZTERMINATOR_H
#define DZTERMINATOR_H

#include "DzValue.h"
#include "FunctionAttribute.h"

class DzTerminator : public DzValue
{
	public:
		DzTerminator(const std::string &name, FunctionAttribute attribute);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

		FunctionAttribute attribute() const;

		static DzTerminator *instance();

	private:
		std::string m_name;

		FunctionAttribute m_attribute;
};

#endif // DZTERMINATOR_H
