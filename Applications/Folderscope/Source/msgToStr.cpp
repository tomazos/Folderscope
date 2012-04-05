// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "msgToStr.h"

Ascii msgToStr(UINT uMsg)
{
	switch (uMsg)
	{
	#define DefnMsg(X) case X: return #X;
    #ifdef WM_ACTIVATE
        DefnMsg(WM_ACTIVATE)
    #endif
    #ifdef WM_ACTIVATEAPP
        DefnMsg(WM_ACTIVATEAPP)
    #endif
    #ifdef WM_AFXFIRST
        DefnMsg(WM_AFXFIRST)
    #endif
    #ifdef WM_AFXLAST
        DefnMsg(WM_AFXLAST)
    #endif
    #ifdef WM_APP
        DefnMsg(WM_APP)
    #endif
    #ifdef WM_APPCOMMAND
        DefnMsg(WM_APPCOMMAND)
    #endif
    #ifdef WM_ASKCBFORMATNAME
        DefnMsg(WM_ASKCBFORMATNAME)
    #endif
    #ifdef WM_CANCELJOURNAL
        DefnMsg(WM_CANCELJOURNAL)
    #endif
    #ifdef WM_CANCELMODE
        DefnMsg(WM_CANCELMODE)
    #endif
    #ifdef WM_CAPTURECHANGED
        DefnMsg(WM_CAPTURECHANGED)
    #endif
    #ifdef WM_CHANGECBCHAIN
        DefnMsg(WM_CHANGECBCHAIN)
    #endif
    #ifdef WM_CHANGEUISTATE
        DefnMsg(WM_CHANGEUISTATE)
    #endif
    #ifdef WM_CHAR
        DefnMsg(WM_CHAR)
    #endif
    #ifdef WM_CHARTOITEM
        DefnMsg(WM_CHARTOITEM)
    #endif
    #ifdef WM_CHILDACTIVATE
        DefnMsg(WM_CHILDACTIVATE)
    #endif
    #ifdef WM_CLEAR
        DefnMsg(WM_CLEAR)
    #endif
    #ifdef WM_CLIPBOARDUPDATE
        DefnMsg(WM_CLIPBOARDUPDATE)
    #endif
    #ifdef WM_CLOSE
        DefnMsg(WM_CLOSE)
    #endif
    #ifdef WM_COMMAND
        DefnMsg(WM_COMMAND)
    #endif
    #ifdef WM_COMMNOTIFY
        DefnMsg(WM_COMMNOTIFY)
    #endif
    #ifdef WM_COMPACTING
        DefnMsg(WM_COMPACTING)
    #endif
    #ifdef WM_COMPAREITEM
        DefnMsg(WM_COMPAREITEM)
    #endif
    #ifdef WM_CONTEXTMENU
        DefnMsg(WM_CONTEXTMENU)
    #endif
    #ifdef WM_COPY
        DefnMsg(WM_COPY)
    #endif
    #ifdef WM_COPYDATA
        DefnMsg(WM_COPYDATA)
    #endif
    #ifdef WM_CREATE
        DefnMsg(WM_CREATE)
    #endif
    #ifdef WM_CTLCOLORBTN
        DefnMsg(WM_CTLCOLORBTN)
    #endif
    #ifdef WM_CTLCOLORDLG
        DefnMsg(WM_CTLCOLORDLG)
    #endif
    #ifdef WM_CTLCOLOREDIT
        DefnMsg(WM_CTLCOLOREDIT)
    #endif
    #ifdef WM_CTLCOLORLISTBOX
        DefnMsg(WM_CTLCOLORLISTBOX)
    #endif
    #ifdef WM_CTLCOLORMSGBOX
        DefnMsg(WM_CTLCOLORMSGBOX)
    #endif
    #ifdef WM_CTLCOLORSCROLLBAR
        DefnMsg(WM_CTLCOLORSCROLLBAR)
    #endif
    #ifdef WM_CTLCOLORSTATIC
        DefnMsg(WM_CTLCOLORSTATIC)
    #endif
    #ifdef WM_CUT
        DefnMsg(WM_CUT)
    #endif
    #ifdef WM_DEADCHAR
        DefnMsg(WM_DEADCHAR)
    #endif
    #ifdef WM_DELETEITEM
        DefnMsg(WM_DELETEITEM)
    #endif
    #ifdef WM_DESTROY
        DefnMsg(WM_DESTROY)
    #endif
    #ifdef WM_DESTROYCLIPBOARD
        DefnMsg(WM_DESTROYCLIPBOARD)
    #endif
    #ifdef WM_DEVICECHANGE
        DefnMsg(WM_DEVICECHANGE)
    #endif
    #ifdef WM_DEVMODECHANGE
        DefnMsg(WM_DEVMODECHANGE)
    #endif
    #ifdef WM_DISPLAYCHANGE
        DefnMsg(WM_DISPLAYCHANGE)
    #endif
    #ifdef WM_DRAWCLIPBOARD
        DefnMsg(WM_DRAWCLIPBOARD)
    #endif
    #ifdef WM_DRAWITEM
        DefnMsg(WM_DRAWITEM)
    #endif
    #ifdef WM_DROPFILES
        DefnMsg(WM_DROPFILES)
    #endif
    #ifdef WM_DWMCOLORIZATIONCOLORCHANGED
        DefnMsg(WM_DWMCOLORIZATIONCOLORCHANGED)
    #endif
    #ifdef WM_DWMCOMPOSITIONCHANGED
        DefnMsg(WM_DWMCOMPOSITIONCHANGED)
    #endif
    #ifdef WM_DWMNCRENDERINGCHANGED
        DefnMsg(WM_DWMNCRENDERINGCHANGED)
    #endif
    #ifdef WM_DWMWINDOWMAXIMIZEDCHANGE
        DefnMsg(WM_DWMWINDOWMAXIMIZEDCHANGE)
    #endif
    #ifdef WM_ENABLE
        DefnMsg(WM_ENABLE)
    #endif
    #ifdef WM_ENDSESSION
        DefnMsg(WM_ENDSESSION)
    #endif
    #ifdef WM_ENTERIDLE
        DefnMsg(WM_ENTERIDLE)
    #endif
    #ifdef WM_ENTERMENULOOP
        DefnMsg(WM_ENTERMENULOOP)
    #endif
    #ifdef WM_ENTERSIZEMOVE
        DefnMsg(WM_ENTERSIZEMOVE)
    #endif
    #ifdef WM_ERASEBACKGROUND
        DefnMsg(WM_ERASEBACKGROUND)
    #endif
    #ifdef WM_ERASEBKGND
        DefnMsg(WM_ERASEBKGND)
    #endif
    #ifdef WM_EXITMENULOOP
        DefnMsg(WM_EXITMENULOOP)
    #endif
    #ifdef WM_EXITSIZEMOVE
        DefnMsg(WM_EXITSIZEMOVE)
    #endif
    #ifdef WM_FONTCHANGE
        DefnMsg(WM_FONTCHANGE)
    #endif
    #ifdef WM_GETDLGCODE
        DefnMsg(WM_GETDLGCODE)
    #endif
    #ifdef WM_GETFONT
        DefnMsg(WM_GETFONT)
    #endif
    #ifdef WM_GETHOTKEY
        DefnMsg(WM_GETHOTKEY)
    #endif
    #ifdef WM_GETICON
        DefnMsg(WM_GETICON)
    #endif
    #ifdef WM_GETMINMAXINFO
        DefnMsg(WM_GETMINMAXINFO)
    #endif
    #ifdef WM_GETOBJECT
        DefnMsg(WM_GETOBJECT)
    #endif
    #ifdef WM_GETTEXT
        DefnMsg(WM_GETTEXT)
    #endif
    #ifdef WM_GETTEXTLENGTH
        DefnMsg(WM_GETTEXTLENGTH)
    #endif
    #ifdef WM_GETTITLEBARINFOEX
        DefnMsg(WM_GETTITLEBARINFOEX)
    #endif
    #ifdef WM_HANDHELDFIRST
        DefnMsg(WM_HANDHELDFIRST)
    #endif
    #ifdef WM_HANDHELDLAST
        DefnMsg(WM_HANDHELDLAST)
    #endif
    #ifdef WM_HELP
        DefnMsg(WM_HELP)
    #endif
    #ifdef WM_HOTKEY
        DefnMsg(WM_HOTKEY)
    #endif
    #ifdef WM_HSCROLL
        DefnMsg(WM_HSCROLL)
    #endif
    #ifdef WM_HSCROLLCLIPBOARD
        DefnMsg(WM_HSCROLLCLIPBOARD)
    #endif
    #ifdef WM_ICONERASEBKGND
        DefnMsg(WM_ICONERASEBKGND)
    #endif
    #ifdef WM_IME_CHAR
        DefnMsg(WM_IME_CHAR)
    #endif
    #ifdef WM_IME_COMPOSITION
        DefnMsg(WM_IME_COMPOSITION)
    #endif
    #ifdef WM_IME_COMPOSITIONFULL
        DefnMsg(WM_IME_COMPOSITIONFULL)
    #endif
    #ifdef WM_IME_CONTROL
        DefnMsg(WM_IME_CONTROL)
    #endif
    #ifdef WM_IME_ENDCOMPOSITION
        DefnMsg(WM_IME_ENDCOMPOSITION)
    #endif
    #ifdef WM_IME_KEYDOWN
        DefnMsg(WM_IME_KEYDOWN)
    #endif
    #ifdef WM_IME_KEYUP
        DefnMsg(WM_IME_KEYUP)
    #endif
    #ifdef WM_IME_NOTIFY
        DefnMsg(WM_IME_NOTIFY)
    #endif
    #ifdef WM_IME_REQUEST
        DefnMsg(WM_IME_REQUEST)
    #endif
    #ifdef WM_IME_SELECT
        DefnMsg(WM_IME_SELECT)
    #endif
    #ifdef WM_IME_SETCONTEXT
        DefnMsg(WM_IME_SETCONTEXT)
    #endif
    #ifdef WM_IME_STARTCOMPOSITION
        DefnMsg(WM_IME_STARTCOMPOSITION)
    #endif
    #ifdef WM_INITDIALOG
        DefnMsg(WM_INITDIALOG)
    #endif
    #ifdef WM_INITMENU
        DefnMsg(WM_INITMENU)
    #endif
    #ifdef WM_INITMENUPOPUP
        DefnMsg(WM_INITMENUPOPUP)
    #endif
    #ifdef WM_INPUT
        DefnMsg(WM_INPUT)
    #endif
    #ifdef WM_INPUTLANGCHANGE
        DefnMsg(WM_INPUTLANGCHANGE)
    #endif
    #ifdef WM_INPUTLANGCHANGEREQUEST
        DefnMsg(WM_INPUTLANGCHANGEREQUEST)
    #endif
    #ifdef WM_INPUT_DEVICE_CHANGE
        DefnMsg(WM_INPUT_DEVICE_CHANGE)
    #endif
    #ifdef WM_KEYDOWN
        DefnMsg(WM_KEYDOWN)
    #endif
    #ifdef WM_KEYLAST
        DefnMsg(WM_KEYLAST)
    #endif
    #ifdef WM_KEYUP
        DefnMsg(WM_KEYUP)
    #endif
    #ifdef WM_KILLFOCUS
        DefnMsg(WM_KILLFOCUS)
    #endif
    #ifdef WM_LBUTTONDBLCLK
        DefnMsg(WM_LBUTTONDBLCLK)
    #endif
    #ifdef WM_LBUTTONDOWN
        DefnMsg(WM_LBUTTONDOWN)
    #endif
    #ifdef WM_LBUTTONUP
        DefnMsg(WM_LBUTTONUP)
    #endif
    #ifdef WM_MBUTTONDBLCLK
        DefnMsg(WM_MBUTTONDBLCLK)
    #endif
    #ifdef WM_MBUTTONDOWN
        DefnMsg(WM_MBUTTONDOWN)
    #endif
    #ifdef WM_MBUTTONUP
        DefnMsg(WM_MBUTTONUP)
    #endif
    #ifdef WM_MDIACTIVATE
        DefnMsg(WM_MDIACTIVATE)
    #endif
    #ifdef WM_MDICASCADE
        DefnMsg(WM_MDICASCADE)
    #endif
    #ifdef WM_MDICREATE
        DefnMsg(WM_MDICREATE)
    #endif
    #ifdef WM_MDIDESTROY
        DefnMsg(WM_MDIDESTROY)
    #endif
    #ifdef WM_MDIGETACTIVE
        DefnMsg(WM_MDIGETACTIVE)
    #endif
    #ifdef WM_MDIICONARRANGE
        DefnMsg(WM_MDIICONARRANGE)
    #endif
    #ifdef WM_MDIMAXIMIZE
        DefnMsg(WM_MDIMAXIMIZE)
    #endif
    #ifdef WM_MDINEXT
        DefnMsg(WM_MDINEXT)
    #endif
    #ifdef WM_MDIREFRESHMENU
        DefnMsg(WM_MDIREFRESHMENU)
    #endif
    #ifdef WM_MDIRESTORE
        DefnMsg(WM_MDIRESTORE)
    #endif
    #ifdef WM_MDISETMENU
        DefnMsg(WM_MDISETMENU)
    #endif
    #ifdef WM_MDITILE
        DefnMsg(WM_MDITILE)
    #endif
    #ifdef WM_MEASUREITEM
        DefnMsg(WM_MEASUREITEM)
    #endif
    #ifdef WM_MENUCHAR
        DefnMsg(WM_MENUCHAR)
    #endif
    #ifdef WM_MENUCOMMAND
        DefnMsg(WM_MENUCOMMAND)
    #endif
    #ifdef WM_MENUDRAG
        DefnMsg(WM_MENUDRAG)
    #endif
    #ifdef WM_MENUGETOBJECT
        DefnMsg(WM_MENUGETOBJECT)
    #endif
    #ifdef WM_MENURBUTTONUP
        DefnMsg(WM_MENURBUTTONUP)
    #endif
    #ifdef WM_MENUSELECT
        DefnMsg(WM_MENUSELECT)
    #endif
    #ifdef WM_MOUSEACTIVATE
        DefnMsg(WM_MOUSEACTIVATE)
    #endif
    #ifdef WM_MOUSEFIRST
        DefnMsg(WM_MOUSEFIRST)
    #endif
    #ifdef WM_MOUSEHOVER
        DefnMsg(WM_MOUSEHOVER)
    #endif
    #ifdef WM_MOUSEHWHEEL
        DefnMsg(WM_MOUSEHWHEEL)
    #endif
    #ifdef WM_MOUSELEAVE
        DefnMsg(WM_MOUSELEAVE)
    #endif
    #ifdef WM_MOUSEWHEEL
        DefnMsg(WM_MOUSEWHEEL)
    #endif
    #ifdef WM_MOVE
        DefnMsg(WM_MOVE)
    #endif
    #ifdef WM_MOVING
        DefnMsg(WM_MOVING)
    #endif
    #ifdef WM_NCACTIVATE
        DefnMsg(WM_NCACTIVATE)
    #endif
    #ifdef WM_NCCALCSIZE
        DefnMsg(WM_NCCALCSIZE)
    #endif
    #ifdef WM_NCCREATE
        DefnMsg(WM_NCCREATE)
    #endif
    #ifdef WM_NCDESTROY
        DefnMsg(WM_NCDESTROY)
    #endif
    #ifdef WM_NCHITTEST
        DefnMsg(WM_NCHITTEST)
    #endif
    #ifdef WM_NCLBUTTONDBLCLK
        DefnMsg(WM_NCLBUTTONDBLCLK)
    #endif
    #ifdef WM_NCLBUTTONDOWN
        DefnMsg(WM_NCLBUTTONDOWN)
    #endif
    #ifdef WM_NCLBUTTONUP
        DefnMsg(WM_NCLBUTTONUP)
    #endif
    #ifdef WM_NCMBUTTONDBLCLK
        DefnMsg(WM_NCMBUTTONDBLCLK)
    #endif
    #ifdef WM_NCMBUTTONDOWN
        DefnMsg(WM_NCMBUTTONDOWN)
    #endif
    #ifdef WM_NCMBUTTONUP
        DefnMsg(WM_NCMBUTTONUP)
    #endif
    #ifdef WM_NCMOUSEHOVER
        DefnMsg(WM_NCMOUSEHOVER)
    #endif
    #ifdef WM_NCMOUSELEAVE
        DefnMsg(WM_NCMOUSELEAVE)
    #endif
    #ifdef WM_NCMOUSEMOVE
        DefnMsg(WM_NCMOUSEMOVE)
    #endif
    #ifdef WM_NCPAINT
        DefnMsg(WM_NCPAINT)
    #endif
    #ifdef WM_NCRBUTTONDBLCLK
        DefnMsg(WM_NCRBUTTONDBLCLK)
    #endif
    #ifdef WM_NCRBUTTONDOWN
        DefnMsg(WM_NCRBUTTONDOWN)
    #endif
    #ifdef WM_NCRBUTTONUP
        DefnMsg(WM_NCRBUTTONUP)
    #endif
    #ifdef WM_NCXBUTTONDBLCLK
        DefnMsg(WM_NCXBUTTONDBLCLK)
    #endif
    #ifdef WM_NCXBUTTONDOWN
        DefnMsg(WM_NCXBUTTONDOWN)
    #endif
    #ifdef WM_NCXBUTTONUP
        DefnMsg(WM_NCXBUTTONUP)
    #endif
    #ifdef WM_NEXTDLGCTL
        DefnMsg(WM_NEXTDLGCTL)
    #endif
    #ifdef WM_NEXTMENU
        DefnMsg(WM_NEXTMENU)
    #endif
    #ifdef WM_NOTIFY
        DefnMsg(WM_NOTIFY)
    #endif
    #ifdef WM_NOTIFYFORMAT
        DefnMsg(WM_NOTIFYFORMAT)
    #endif
    #ifdef WM_NULL
        DefnMsg(WM_NULL)
    #endif
    #ifdef WM_PAINT
        DefnMsg(WM_PAINT)
    #endif
    #ifdef WM_PAINTCLIPBOARD
        DefnMsg(WM_PAINTCLIPBOARD)
    #endif
    #ifdef WM_PAINTICON
        DefnMsg(WM_PAINTICON)
    #endif
    #ifdef WM_PALETTECHANGED
        DefnMsg(WM_PALETTECHANGED)
    #endif
    #ifdef WM_PALETTEISCHANGING
        DefnMsg(WM_PALETTEISCHANGING)
    #endif
    #ifdef WM_PARENTNOTIFY
        DefnMsg(WM_PARENTNOTIFY)
    #endif
    #ifdef WM_PASTE
        DefnMsg(WM_PASTE)
    #endif
    #ifdef WM_PENWINFIRST
        DefnMsg(WM_PENWINFIRST)
    #endif
    #ifdef WM_PENWINLAST
        DefnMsg(WM_PENWINLAST)
    #endif
    #ifdef WM_POWER
        DefnMsg(WM_POWER)
    #endif
    #ifdef WM_POWERBROADCAST
        DefnMsg(WM_POWERBROADCAST)
    #endif
    #ifdef WM_PRINT
        DefnMsg(WM_PRINT)
    #endif
    #ifdef WM_PRINTCLIENT
        DefnMsg(WM_PRINTCLIENT)
    #endif
    #ifdef WM_QUERYDRAGICON
        DefnMsg(WM_QUERYDRAGICON)
    #endif
    #ifdef WM_QUERYENDSESSION
        DefnMsg(WM_QUERYENDSESSION)
    #endif
    #ifdef WM_QUERYNEWPALETTE
        DefnMsg(WM_QUERYNEWPALETTE)
    #endif
    #ifdef WM_QUERYOPEN
        DefnMsg(WM_QUERYOPEN)
    #endif
    #ifdef WM_QUERYUISTATE
        DefnMsg(WM_QUERYUISTATE)
    #endif
    #ifdef WM_QUEUESYNC
        DefnMsg(WM_QUEUESYNC)
    #endif
    #ifdef WM_QUIT
        DefnMsg(WM_QUIT)
    #endif
    #ifdef WM_RBUTTONDBLCLK
        DefnMsg(WM_RBUTTONDBLCLK)
    #endif
    #ifdef WM_RBUTTONDOWN
        DefnMsg(WM_RBUTTONDOWN)
    #endif
    #ifdef WM_RBUTTONUP
        DefnMsg(WM_RBUTTONUP)
    #endif
    #ifdef WM_RENDERALLFORMATS
        DefnMsg(WM_RENDERALLFORMATS)
    #endif
    #ifdef WM_RENDERFORMAT
        DefnMsg(WM_RENDERFORMAT)
    #endif
    #ifdef WM_SETCURSOR
        DefnMsg(WM_SETCURSOR)
    #endif
    #ifdef WM_SETFOCUS
        DefnMsg(WM_SETFOCUS)
    #endif
    #ifdef WM_SETFONT
        DefnMsg(WM_SETFONT)
    #endif
    #ifdef WM_SETHOTKEY
        DefnMsg(WM_SETHOTKEY)
    #endif
    #ifdef WM_SETICON
        DefnMsg(WM_SETICON)
    #endif
    #ifdef WM_SETREDRAW
        DefnMsg(WM_SETREDRAW)
    #endif
    #ifdef WM_SETTEXT
        DefnMsg(WM_SETTEXT)
    #endif
    #ifdef WM_SHOWWINDOW
        DefnMsg(WM_SHOWWINDOW)
    #endif
    #ifdef WM_SIZE
        DefnMsg(WM_SIZE)
    #endif
    #ifdef WM_SIZECLIPBOARD
        DefnMsg(WM_SIZECLIPBOARD)
    #endif
    #ifdef WM_SIZING
        DefnMsg(WM_SIZING)
    #endif
    #ifdef WM_SPOOLERSTATUS
        DefnMsg(WM_SPOOLERSTATUS)
    #endif
    #ifdef WM_STYLECHANGED
        DefnMsg(WM_STYLECHANGED)
    #endif
    #ifdef WM_STYLECHANGING
        DefnMsg(WM_STYLECHANGING)
    #endif
    #ifdef WM_SYNCPAINT
        DefnMsg(WM_SYNCPAINT)
    #endif
    #ifdef WM_SYSCHAR
        DefnMsg(WM_SYSCHAR)
    #endif
    #ifdef WM_SYSCOLORCHANGE
        DefnMsg(WM_SYSCOLORCHANGE)
    #endif
    #ifdef WM_SYSCOMMAND
        DefnMsg(WM_SYSCOMMAND)
    #endif
    #ifdef WM_SYSDEADCHAR
        DefnMsg(WM_SYSDEADCHAR)
    #endif
    #ifdef WM_SYSKEYDOWN
        DefnMsg(WM_SYSKEYDOWN)
    #endif
    #ifdef WM_SYSKEYUP
        DefnMsg(WM_SYSKEYUP)
    #endif
    #ifdef WM_TABLET_FIRST
        DefnMsg(WM_TABLET_FIRST)
    #endif
    #ifdef WM_TABLET_LAST
        DefnMsg(WM_TABLET_LAST)
    #endif
    #ifdef WM_TCARD
        DefnMsg(WM_TCARD)
    #endif
    #ifdef WM_THEMECHANGED
        DefnMsg(WM_THEMECHANGED)
    #endif
    #ifdef WM_TIMECHANGE
        DefnMsg(WM_TIMECHANGE)
    #endif
    #ifdef WM_TIMER
        DefnMsg(WM_TIMER)
    #endif
    #ifdef WM_UNDO
        DefnMsg(WM_UNDO)
    #endif
    #ifdef WM_UNINITMENUPOPUP
        DefnMsg(WM_UNINITMENUPOPUP)
    #endif
    #ifdef WM_UPDATEUISTATE
        DefnMsg(WM_UPDATEUISTATE)
    #endif
    #ifdef WM_USER
        DefnMsg(WM_USER)
    #endif
    #ifdef WM_USERCHANGED
        DefnMsg(WM_USERCHANGED)
    #endif
    #ifdef WM_VKEYTOITEM
        DefnMsg(WM_VKEYTOITEM)
    #endif
    #ifdef WM_VSCROLL
        DefnMsg(WM_VSCROLL)
    #endif
    #ifdef WM_VSCROLLCLIPBOARD
        DefnMsg(WM_VSCROLLCLIPBOARD)
    #endif
    #ifdef WM_WINDOWPOSCHANGED
        DefnMsg(WM_WINDOWPOSCHANGED)
    #endif
    #ifdef WM_WINDOWPOSCHANGING
        DefnMsg(WM_WINDOWPOSCHANGING)
    #endif
    #ifdef WM_WININICHANGE
        DefnMsg(WM_WININICHANGE)
    #endif
    #ifdef WM_WTSSESSION_CHANGE
        DefnMsg(WM_WTSSESSION_CHANGE)
    #endif
    #ifdef WM_XBUTTONDBLCLK
        DefnMsg(WM_XBUTTONDBLCLK)
    #endif
    #ifdef WM_XBUTTONDOWN
        DefnMsg(WM_XBUTTONDOWN)
    #endif
    #ifdef WM_XBUTTONUP
        DefnMsg(WM_XBUTTONUP)
    #endif
	#undef DefnMsg
	default: return Ascii::format("WM_?(%x)", uMsg);
	}
}