#ifndef ITYPENAME_H
#define ITYPENAME_H

#include <string>

class EntryPoint;
class Type;

class ITypeName
{
	public:
		virtual Type *resolve(const EntryPoint &entryPoint) const = 0;
};

#endif // ITYPENAME_H
