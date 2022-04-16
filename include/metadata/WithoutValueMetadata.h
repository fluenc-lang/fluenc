#ifndef WITHOUTVALUEMETADATA_H
#define WITHOUTVALUEMETADATA_H

#include "ValueMetadata.h"

class WithoutValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // WITHOUTVALUEMETADATA_H
