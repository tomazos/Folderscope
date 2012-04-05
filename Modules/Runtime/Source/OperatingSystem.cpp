// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/OperatingSystem.h"

#include "shellapi.h"

void OperatingSystem::open(Error& error, const UTF16& sTarget)
{
	int iRes = (int) ShellExecute(NULL, L"open", sTarget.ptr(), NULL, NULL, SW_SHOW);

	if (iRes <= 32)
	{
		const Chr16* sShellExecuteError = L"Unknown ShellExecute Error";

		switch (iRes)
		{
		case 0: sShellExecuteError = L"The operating system is out of memory or resources."; break;
		case ERROR_FILE_NOT_FOUND: sShellExecuteError = L"The specified file was not found."; break;
		case ERROR_PATH_NOT_FOUND: sShellExecuteError = L"The specified path was not found."; break;
		case ERROR_BAD_FORMAT: sShellExecuteError = L"The .exe file is invalid (non-Microsoft Win32 .exe or error in .exe image)."; break;
		case SE_ERR_ACCESSDENIED: sShellExecuteError = L"The operating system denied access to the specified file."; break;
		case SE_ERR_ASSOCINCOMPLETE: sShellExecuteError = L"The file name association is incomplete or invalid."; break;
		case SE_ERR_DDEBUSY: sShellExecuteError = L"The Dynamic Data Exchange (DDE) transaction could not be completed because other DDE transactions were being processed."; break;
		case SE_ERR_DDEFAIL: sShellExecuteError = L"The DDE transaction failed."; break;
		case SE_ERR_DDETIMEOUT: sShellExecuteError = L"The DDE transaction could not be completed because the request timed out."; break;
		case SE_ERR_DLLNOTFOUND: sShellExecuteError = L"The specified DLL was not found."; break;
		case SE_ERR_NOASSOC: sShellExecuteError = L"There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable."; break;
		case SE_ERR_OOM: sShellExecuteError = L"There was not enough memory to complete the operation."; break;
		case SE_ERR_SHARE: sShellExecuteError = L"A sharing violation occurred."; break;
		}

		error.what(sShellExecuteError);
		error.who(L"ShellExecute");
	}
};

