// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Runtime.h"

#include "GUI.h"

DeclWeakHandleable(ProgressBar);
class ProgressBar : public GUI
{
public:
	ProgressBar(GUI* pParent);
	virtual ~ProgressBar();

	void setProgress(Real64 fProgress);
};
