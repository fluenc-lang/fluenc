#ifndef SCALARVALUEMETADATA_H
#define SCALARVALUEMETADATA_H

#include "ValueMetadata.h"

class ScalarValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // SCALARVALUEMETADATA_H
