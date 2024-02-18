#ifndef ITERATABLE_H
#define ITERATABLE_H

#include "fragment.hpp"

class Type;
class Emitter;

class Iteratable : public fragment
{
	friend class Emitter;

	public:
		Iteratable(const fragment *subject, const Type *type);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const fragment *m_subject;
		const Type *m_type;
};

#endif // ITERATABLE_H
