#ifndef INDEXEDVALUEMETADATA_H
#define INDEXEDVALUEMETADATA_H

#include "ValueMetadata.h"

class IndexedValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // INDEXEDVALUEMETADATA_H
