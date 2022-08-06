#ifndef DZRESULT_H
#define DZRESULT_H

#include "EntryPoint.h"
#include "Stack.h"

struct DzResult
{
	EntryPoint entryPoint;
	Stack values;
};

#endif // DZRESULT_H
