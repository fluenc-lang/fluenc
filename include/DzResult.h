#ifndef DZRESULT_H
#define DZRESULT_H

#include "EntryPoint.h"
#include "Stack.h"

template<typename T>
struct DzResult
{
	EntryPoint entryPoint;
	Stack<T> values;
};

#endif // DZRESULT_H
