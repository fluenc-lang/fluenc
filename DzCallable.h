#ifndef DZCALLABLE_H
#define DZCALLABLE_H

#include "DzValueNg.h"
#include "FunctionAttribute.h"

class DzMemberNg;
class DzTypeNameNg;

class DzCallable : public DzValueNg
{
	public:
		virtual std::string name() const = 0;
		virtual std::vector<DzMemberNg *> arguments() const = 0;

		virtual DzTypeNameNg *returnType() const = 0;

		virtual FunctionAttribute attribute() const = 0;
};

#endif // DZCALLABLE_H
