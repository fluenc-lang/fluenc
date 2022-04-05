#ifndef INSTANTIATION_H
#define INSTANTIATION_H

#include <unordered_set>

#include <llvm/IR/Instructions.h>

#include "Node.h"
#include "DzTypeName.h"

class DzAssignment;
class IPrototypeProvider;

class Instantiation : public Node
{
	public:
		Instantiation(Node *consumer
			, IPrototypeProvider *prototypeProvider
			, const std::vector<std::string> &fields
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
		IPrototypeProvider *m_prototypeProvider;

		std::vector<std::string> m_fields;
};

#endif // INSTANTIATION_H
