#ifndef STRINGVALUEMETADATA_H
#define STRINGVALUEMETADATA_H

#include "ValueMetadata.h"

class StringValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // STRINGVALUEMETADATA_H
