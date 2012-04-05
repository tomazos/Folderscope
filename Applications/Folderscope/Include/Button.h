// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Runtime.h"

#include "GUI.h"

DeclWeakHandleable(Button);
class Button : public GUI
{
public:
	Button(GUI* pParent);
	virtual ~Button();

	void click();

protected:

	virtual void onClick() = 0;
	virtual Result note_clicked();
};
