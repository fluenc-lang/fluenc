#ifndef IPROTOTYPE_H
#define IPROTOTYPE_H

#include "Type.h"
#include "PrototypeField.h"

class EntryPoint;

class IPrototype : public Type
{
	public:
		TypeKind kind() const override;

		virtual std::vector<PrototypeField> fields(const EntryPoint &entryPoint) const = 0;
};

#endif // IPROTOTYPE_H
