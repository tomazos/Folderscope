// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Runtime.h"

#include "GUI.h"

DeclWeakHandleable(EditField);
class EditField : public GUI
{
public:
	EditField(GUI* pParent);
	virtual ~EditField();

	void setCueBanner(const UTF16& sBanner);
	void getSel(int& iStartSel, int& iEndSel);
	void setSel(int iStartSel, int iEndSel);

protected:
	virtual bool approve(const UTF16& sNewText) { return true; }

private:

	virtual void onChange() {};
	virtual void onFocus() {};
	virtual void onBlur() {};

	virtual Result note_setfocus();
	virtual Result note_killfocus();
	virtual Result note_change();
};
