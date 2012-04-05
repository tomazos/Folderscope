// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "WinMain.h"
#include "Basics/DebugMemory.h"

#include "commctrl.h"

#include "Runtime.h"

#include "Application.h"

#include "Winsock2.h"
#include "ole2.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

static void initStandardControls()
{
	INITCOMMONCONTROLSEX InitControls;
    InitControls.dwSize = sizeof (InitControls);
    InitControls.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx (&InitControls);
};

static void initWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

static void shutdownWinsock()
{
	WSACleanup();
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	Lock::setupGlobalLock();

	#ifdef MY_DEBUG_MEMORY
		DebugMemory::instance();
	#endif

	GUI::s_hinstance = hInstance;

	initStandardControls();
	OleInitialize(NULL);
	initWinsock();
	
	HApplication app = NewObject(Application);

	MSG msg;
    while (true)
    { 
		int iResult = GetMessage( &msg, NULL, 0, 0 );

		if (iResult == 0) // WM_QUIT
			break;

        if (iResult == -1)
		{
			GetLastError();
			Log::errorf(L"WinMain, GetMessage returned -1, GetLastError() = %lx", GetLastError());
            break;
		}

        TranslateMessage(&msg); 
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			break;
	}

	Application::s_hFileLog.setNull();

	app.setNull();

	shutdownWinsock();

    return msg.wParam; 
};
