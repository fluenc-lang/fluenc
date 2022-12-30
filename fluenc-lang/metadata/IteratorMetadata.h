#ifndef ITERATORMETADATA_H
#define ITERATORMETADATA_H

#include "ValueMetadata.h"

class IteratorMetadata : public ValueMetadata
{
	public:
		std::string name() const;
};

#endif // ITERATORMETADATA_H
