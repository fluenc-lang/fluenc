#ifndef PARENTINJECTORNODE_H
#define PARENTINJECTORNODE_H

#include "IBlockInstruction.h"

class ParentInjectorNode : public IBlockInstruction
{
	public:
		ParentInjectorNode(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType>>> &nodes
			, const std::vector<std::string> &namespaces
			, const Type *iteratorType
			, const Node *alpha
			, const Node *beta
			);

		bool containsIterator() const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		static const IBlockInstruction *inject(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType>>> &nodes
			, const std::vector<std::string> &namespaces
			, const Type *iteratorType
			, const Node *kask
			, const Node *alpha
			, const Node *beta
			);

		const IBlockInstruction *m_subject;
};


#endif // PARENTINJECTOR_H
