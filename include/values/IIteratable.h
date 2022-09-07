#ifndef IITERATABLE_H
#define IITERATABLE_H

#include "Emitter.h"
#include "Analyzer.h"
#include "Visitable.h"

class EntryPoint;

class IIteratable : public Visitable<Emitter, Analyzer>
{
};

#endif // IITERATABLE_H
