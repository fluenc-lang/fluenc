#ifndef VALUEHELPER_H
#define VALUEHELPER_H

#include "EntryPoint.h"

class BaseValue;
class ScalarValue;

class ValueHelper
{
	public:
		static EntryPoint transferValue(const EntryPoint &entryPoint
			, const BaseValue *value
			, const BaseValue *storage
			);

		static const ScalarValue *getScalar(const EntryPoint &entryPoint, Stack &values);
};

#endif // VALUEHELPER_H
