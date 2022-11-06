#ifndef ITERATORSTORAGEBOUNDARY_H
#define ITERATORSTORAGEBOUNDARY_H

#include "CallableNode.h"

class IteratorStorageBoundary : public CallableNode
{
	public:
		IteratorStorageBoundary(const CallableNode *subject);

		int order(const EntryPoint &entryPoint) const override;

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		const CallableNode *m_subject;
};

#endif // ITERATORSTORAGEBOUNDARY_H
