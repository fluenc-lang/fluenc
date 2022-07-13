#ifndef STRINGITERATABLE_H
#define STRINGITERATABLE_H

#include "values/IIteratable.h"

class StringIteratable : public IIteratable
{
	friend class Emitter;

	public:
		StringIteratable(const ReferenceValue *index, const Node *node, llvm::Value *address, size_t length);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const ReferenceValue *m_index;
		const Node *m_node;

		llvm::Value *m_address;

		size_t m_length;
};


#endif // STRINGITERATABLE_H
