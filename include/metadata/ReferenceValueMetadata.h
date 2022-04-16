#ifndef REFERENCEVALUEMETADATA_H
#define REFERENCEVALUEMETADATA_H

#include "ValueMetadata.h"

class ReferenceValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // REFERENCEVALUEMETADATA_H
