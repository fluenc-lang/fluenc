#pragma once

#include "fragment.hpp"

class EntryPoint;

struct stack_segment_node;

class IteratorValue : public fragment
{
	friend class Emitter;

	public:
		IteratorValue(const EntryPoint *entryPoint
			, stack_segment_node *subject
			);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const EntryPoint *m_entryPoint;
		stack_segment_node* m_subject;
};
