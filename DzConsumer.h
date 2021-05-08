#ifndef DZCONSUMER_H
#define DZCONSUMER_H

#include "DzValue.h"

class DzConsumer : public DzValue
{
	public:
		DzConsumer();

		llvm::Value *build(const EntryPoint &entryPoint) const override
		{
			return nullptr;
		}
};

#endif // DZCONSUMER_H
