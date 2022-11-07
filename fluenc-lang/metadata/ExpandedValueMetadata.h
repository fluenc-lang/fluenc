#ifndef EXPANDEDVALUEMETADATA_H
#define EXPANDEDVALUEMETADATA_H

#include "ValueMetadata.h"

class ExpandedValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // EXPANDEDVALUEMETADATA_H
