#ifndef IPROTOTYPE_H
#define IPROTOTYPE_H

#include "Type.h"
#include "PrototypeField.h"

#include "values/NamedValue.h"

class EntryPoint;

class IPrototype : public Type
{
	public:
		virtual std::vector<const NamedValue *> fields(const EntryPoint &entryPoint) const = 0;
};

#endif // IPROTOTYPE_H
