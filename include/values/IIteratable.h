#ifndef IITERATABLE_H
#define IITERATABLE_H

#include "Emitter.h"
#include "Visitable.h"

class EntryPoint;

class IIteratable : public Visitable<Emitter>
{
};

#endif // IITERATABLE_H
