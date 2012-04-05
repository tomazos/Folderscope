// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "ProgressBar.h"

#include "CommCtrl.h"
#include "Richedit.h"

#include "FontProvider.h"

#include "Windowsx.h"

ProgressBar::ProgressBar(GUI* pParent)
	: GUI(pParent, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | PBS_SMOOTH, eDefaultCursor, PROGRESS_CLASS)
{
	SendMessage(m_hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 65535));
	setProgress(0);
}

ProgressBar::~ProgressBar()
{

}

void
ProgressBar::setProgress(Real64 fInProgress)
{
	Real64 fProgress = fInProgress;
	if (fProgress < Real64(0))
		fProgress = Real64(0);
	else if (fProgress > Real64(1))
		fProgress = Real64(1);

	UInt16 iProgress = UInt16(fProgress * Real64(65535));

	SendMessage(m_hwnd, PBM_SETPOS, iProgress, 0);
}
