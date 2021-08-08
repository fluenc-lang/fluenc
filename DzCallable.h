#ifndef DZCALLABLE_H
#define DZCALLABLE_H

#include "DzValue.h"
#include "FunctionAttribute.h"

class DzCallable : public DzValue
{
	public:
		virtual std::string name() const = 0;

		virtual FunctionAttribute attribute() const = 0;
};

#endif // DZCALLABLE_H
