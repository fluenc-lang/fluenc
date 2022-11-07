#include "IteratorStorage.h"

#include "nodes/IteratorStorageBoundary.h"

IteratorStorageBoundary::IteratorStorageBoundary(const CallableNode *subject)
	: m_subject(subject)
{
}

int IteratorStorageBoundary::order(const EntryPoint &entryPoint) const
{
	return m_subject->order(entryPoint);
}

std::string IteratorStorageBoundary::name() const
{
	return m_subject->name();
}

std::vector<DzBaseArgument *> IteratorStorageBoundary::arguments() const
{
	return m_subject->arguments();
}

FunctionAttribute IteratorStorageBoundary::attribute() const
{
	return m_subject->attribute();
}

int8_t IteratorStorageBoundary::signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const
{
	return m_subject->signatureCompatibility(entryPoint, values);
}

std::vector<DzResult> IteratorStorageBoundary::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	auto iteratorStorage = new IteratorStorage();

	auto inputEntryPoint = context.entryPoint
		.withIteratorStorage(iteratorStorage);

	std::vector<DzResult> results;

	for (auto &[entryPoint, values] : m_subject->accept(visitor, { inputEntryPoint, context.values }))
	{
		auto forwardedEntryPoint = entryPoint
			.withIteratorStorage(context.entryPoint.iteratorStorage());

		results.push_back({ forwardedEntryPoint, values });
	}

	return results;
}
