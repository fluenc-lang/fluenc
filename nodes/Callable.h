#ifndef DZCALLABLE_H
#define DZCALLABLE_H

#include "Node.h"
#include "FunctionAttribute.h"

class DzBaseArgument;

class Callable : public Node
{
	public:
		virtual std::string name() const = 0;
		virtual std::vector<DzBaseArgument *> arguments() const = 0;

		virtual FunctionAttribute attribute() const = 0;

		virtual bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const = 0;

		virtual int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const = 0;
};

#endif // DZCALLABLE_H
