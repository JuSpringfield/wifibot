// From: Multi-Thread Programming in C++, Mark Walmsley, Springer
#include <windows.h>
#pragma once

struct Mutex 
{
	Mutex ();
	virtual ~Mutex ();

	virtual void acquire ();
	virtual int acquired ();
	virtual void release ();

private:
	void* mutex;
};