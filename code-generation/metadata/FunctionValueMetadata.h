#ifndef FUNCTIONVALUEMETADATA_H
#define FUNCTIONVALUEMETADATA_H

#include "ValueMetadata.h"

class FunctionValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // FUNCTIONVALUEMETADATA_H
