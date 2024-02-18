#ifndef BUFFERVALUEMETADATA_H
#define BUFFERVALUEMETADATA_H

#include "ValueMetadata.h"

class BufferValueMetadata : public ValueMetadata
{
	public:
		std::string name() const;
};

#endif // BUFFERVALUEMETADATA_H
