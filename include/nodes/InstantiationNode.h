#ifndef INSTANTIATIONNODE_H
#define INSTANTIATIONNODE_H

#include <unordered_set>

#include <llvm/IR/Instructions.h>

#include "Node.h"
#include "DzTypeName.h"

class DzAssignment;
class IPrototypeProvider;

class InstantiationNode : public Node
{
	public:
		InstantiationNode(const std::vector<std::string> &fields
			, const IPrototypeProvider *prototypeProvider
			, const Node *consumer
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<std::string> m_fields;

		const IPrototypeProvider *m_prototypeProvider;
		const Node *m_consumer;
};

#endif // INSTANTIATIONNODE_H
