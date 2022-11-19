#ifndef ARRAYVALUEPROXY_H
#define ARRAYVALUEPROXY_H

#include "IIteratable.h"

class ReferenceValue;

class ArrayValueProxy : public IIteratable
{
	public:
		ArrayValueProxy(const ReferenceValue *index, const IIteratable *subject);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const ReferenceValue *m_index;
		const IIteratable *m_subject;
};

#endif // ARRAYVALUEPROXY_H
