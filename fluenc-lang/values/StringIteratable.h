#ifndef STRINGITERATABLE_H
#define STRINGITERATABLE_H

#include "values/IIteratable.h"

class StringIteratable : public IIteratable
{
	friend class Emitter;

	public:
		StringIteratable(const ReferenceValue *index, const Node *node, llvm::Value *address, size_t length);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const ReferenceValue *m_index;
		const Node *m_node;

		llvm::Value *m_address;

		size_t m_length;
};


#endif // STRINGITERATABLE_H
