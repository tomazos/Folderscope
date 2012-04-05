// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Context.h"

#include "GUI.h"

class DrawingContext: public Context
{
public:
	DrawingContext(GUI* pWindow);
	~DrawingContext();

private:
	HWND m_hwnd;
	PAINTSTRUCT m_ps;
};