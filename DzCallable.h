#ifndef DZCALLABLE_H
#define DZCALLABLE_H

#include "DzValue.h"
#include "FunctionAttribute.h"

class DzBaseArgument;

class DzCallable : public DzValue
{
	public:
		virtual std::string name() const = 0;
		virtual std::vector<DzBaseArgument *> arguments() const = 0;

		virtual FunctionAttribute attribute() const = 0;

		virtual bool hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const = 0;
};

#endif // DZCALLABLE_H
