#ifndef IITERATABLE_H
#define IITERATABLE_H

#include "Emitter.h"
#include "Visitable.h"

class EntryPoint;

class IIteratable : public Visitable<DefaultNodeVisitor>
{
};

#endif // IITERATABLE_H
