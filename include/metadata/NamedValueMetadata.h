#ifndef NAMEDVALUEMETADATA_H
#define NAMEDVALUEMETADATA_H

#include "ValueMetadata.h"

class NamedValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // NAMEDVALUEMETADATA_H
