// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Runtime.h"

#include "GUI.h"

#include "commctrl.h"

DeclWeakHandleable(CheckBox);
class CheckBox : public GUI
{
public:
	CheckBox(GUI* pParent, bool bInitiallyChecked = false);
	virtual ~CheckBox();

	bool isChecked();

	void check();
	void uncheck();

protected:

	virtual void onCheck() {};
	virtual void onUncheck() {};
	virtual Result note_clicked();
};
