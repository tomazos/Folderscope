#pragma once
#include "global.h"

#include "Basics.h"

class FolderImageImpl;

class FolderImageThread : public Thread
{
public:
	FolderImageImpl* imag;
	FolderImageThread(FolderImageImpl* pImag);

	virtual void run();
};
