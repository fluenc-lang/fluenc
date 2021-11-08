#include "DzValue.h"
#include "DzFunctionCall.h"

bool DzValue::compare(const DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

	if (dynamic_cast<const DzFunctionCall *>(other))
	{
		// Very crude. Needs to be improved.

		return !other->compare(this, entryPoint);
	}

	return true;
}
