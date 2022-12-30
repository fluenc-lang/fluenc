#ifndef ITERATABLE_H
#define ITERATABLE_H

#include "IIteratable.h"

class Iteratable : public IIteratable
{
	friend class Emitter;

	public:
		Iteratable(const IIteratable *subject, const Type *type);

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const;

	private:
		const IIteratable *m_subject;
		const Type *m_type;
};

#endif // ITERATABLE_H
