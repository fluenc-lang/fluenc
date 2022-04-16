#ifndef EXPANDABLEVALUEMETADATA_H
#define EXPANDABLEVALUEMETADATA_H

#include "ValueMetadata.h"

class ExpandableValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // EXPANDABLEVALUEMETADATA_H
