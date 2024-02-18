#ifndef STRINGITERATABLE_H
#define STRINGITERATABLE_H

#include "fragment.hpp"

class reference_value;

class StringIteratable : public fragment
{
	friend class Emitter;

	public:
        StringIteratable(const reference_value *index, const reference_value *address, const reference_value *length);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
                const reference_value *m_index;
		const fragment* m_node;

                const reference_value *m_address;
                const reference_value *m_length;
};


#endif // STRINGITERATABLE_H
