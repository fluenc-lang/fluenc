#ifndef READONLYITERATORVALUE_H
#define READONLYITERATORVALUE_H

#include "IIteratable.h"

class ReadOnlyIteratorValue : public IIteratable
{
	public:
		ReadOnlyIteratorValue(const std::vector<DzResult>& results);

		std::vector<DzResult> accept(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const override;

	private:
		std::vector<DzResult> m_results;
};

#endif // READONLYITERATORVALUE_H
