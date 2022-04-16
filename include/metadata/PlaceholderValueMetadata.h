#ifndef PLACEHOLDERVALUEMETADATA_H
#define PLACEHOLDERVALUEMETADATA_H

#include "ValueMetadata.h"

class PlaceholderValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // PLACEHOLDERVALUEMETADATA_H
