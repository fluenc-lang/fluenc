#ifndef VALUEMETADATA_H
#define VALUEMETADATA_H

#include <string>

class ValueMetadata
{
	public:
		virtual std::string name() const = 0;
};

#endif // VALUEMETADATA_H
