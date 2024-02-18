#ifndef USERTYPEVALUEMETADATA_H
#define USERTYPEVALUEMETADATA_H

#include "ValueMetadata.h"

class UserTypeValueMetadata : public ValueMetadata
{
	public:
		std::string name() const override;
};

#endif // USERTYPEVALUEMETADATA_H
