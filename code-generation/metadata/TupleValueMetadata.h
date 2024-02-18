#ifndef TUPLEVALUEMETADATA_H
#define TUPLEVALUEMETADATA_H

#include "ValueMetadata.h"

class TupleValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // TUPLEVALUEMETADATA_H
