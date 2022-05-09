// From: Multi-Thread Programming in C++, Mark Walmsley, Springer

#include "stdafx.h"
#include "mutex.h"


Mutex::Mutex ()
{
	mutex = (void*)CreateMutex (0, false, 0);
}

Mutex::~Mutex ()
{
	CloseHandle (mutex);
}

void Mutex::acquire ()
{
	WaitForSingleObject (mutex, INFINITE);
}

int Mutex::acquired ()
{
	return (WaitForSingleObject (mutex, 0) != WAIT_TIMEOUT);
}

void Mutex::release ()
{
	ReleaseMutex (mutex);
}
