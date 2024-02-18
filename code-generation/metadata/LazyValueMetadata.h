#ifndef LAZYVALUEMETADATA_H
#define LAZYVALUEMETADATA_H

#include "ValueMetadata.h"

class LazyValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // LAZYVALUEMETADATA_H
