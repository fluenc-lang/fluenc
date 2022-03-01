#ifndef VALUEHELPER_H
#define VALUEHELPER_H

#include "EntryPoint.h"

class BaseValue;

class ValueHelper
{
	public:
		static EntryPoint transferValue(const EntryPoint &entryPoint
			, const BaseValue *value
			, const BaseValue *storage
			);
};

#endif // VALUEHELPER_H
