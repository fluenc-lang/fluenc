#ifndef EXPANDEDLAZYVALUEMETADATA_H
#define EXPANDEDLAZYVALUEMETADATA_H

#include "ValueMetadata.h"

class ExpandedLazyValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // EXPANDEDLAZYVALUEMETADATA_H
