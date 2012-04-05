// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Application.h"

#include "Runtime.h"
#include "TabView.h"
#include "LogView.h"
#include "WelcomeView.h"
#include "Menu.h"
#include "Controller.h"

#include "shlobj.h"

#include "TaskTracker.h"

#include "FolderscopeServerRPC.h"
#include "FolderImageView.h"
#include "SurfaceScanner.h"

Application::Application()
{
	s_pInstance = this;

	preStartup();

	m_hMainWindow = NewObject(MainWindow);
	m_hWelcomeView = m_hMainWindow->getWelcomeView();

	TaskTracker::instance()->add(buildMethodTask(this, &Application::postStartup), TaskTracker::eMainThread);
};

Application::~Application()
{
	s_pInstance = NULL;
};

HApplication Application::s_pInstance(NULL);

HLogListener Application::s_hFileLog;

void Application::preStartup()
{
	class StartupFatal {};

	Error error;

	try
	{
		Path appFolder = Path::getAppDataFolder(error);
		if (error) throw StartupFatal();

		Path dcDataFolder = appFolder.subpath(error, L"Folderscope");
		if (error) throw StartupFatal();

		dcDataFolder.vivDirForce(error);
		if (error) throw StartupFatal();

		Path logFolder = dcDataFolder.subpath(error, L"Logs");
		if (error) throw StartupFatal();

		logFolder.vivDirForce(error);
		if (error) throw StartupFatal();

		TimePoint startupTime = TimePoint::now();
		UTF16 sLogSubdir = startupTime.toString("Y-M");
		UTF16 sLogFileName = UTF16(Ascii("Folderscope Log ") + startupTime.toString("Y-M-D h.m.s.t") + ".log");

		Path logSubdir = logFolder.subpath(error, sLogSubdir);
		if (error) throw StartupFatal();

		logSubdir.vivDirForce(error);
		if (error) throw StartupFatal();

		m_logFile = logSubdir.subpath(error, sLogFileName);
		if (error) throw StartupFatal();

		Application::s_hFileLog = NewObject(LogFile, error, m_logFile);
		if (error) throw StartupFatal();

		m_settingsFile = dcDataFolder.subpath(error, L"Settings.json");
		if (error) throw StartupFatal();

		bool bIsFile = m_settingsFile.isFile(error);
		if (error) throw StartupFatal();

		if (!bIsFile)
		{
			ResourceManager::instance()->writeToFile(error, L"DefaultSettings.json", m_settingsFile);
			if (error) throw StartupFatal();
		}

		loadCompiledIn();
		loadSettings();
	
		if (m_jSettings[L"install_id"].isNull())
		{
			RandomInputStream rin(error);
			if (error) throw StartupFatal();

			Buffer buf(64);
			Size iBytesReturned = rin.read(error, buf.ptr(), 64);
			if (error) throw StartupFatal();

			if (iBytesReturned != 64)
			{
				error.what(L"RandomInputStream did not return 64 bytes");
				error.who(L"RandomInputStream::read");
				throw StartupFatal();
			}

			UTF16 sId = HexString::binToHex(Blob(buf));
			m_jSettings[L"install_id"] = JSON(sId);
			saveSettings();
		}
	}
	catch (StartupFatal&)
	{
		MessageBox(NULL, UTF16(error).ptr(), L"Folderscope failed to startup", MB_OK);
		exit(1);
	}
}

void Application::browseFolder()
{
	m_hMainWindow->cmd_open();
}

void Application::postStartup()
{
	loginfo("Welcome to Folderscope %s (C) Tomazos-Jager Software Labs 2009", UTF16(getCurrentVersion()).ptr() );
	loginfo("");
	loginfo("This session is being logged to \"%s\"", UTF16(m_logFile).ptr());
	if (m_jSettings[L"autocheck"].as_bool())
		TaskTracker::instance()->add(buildMethodTask(this, &Application::checkVersion), TaskTracker::eMainThread);
}

void
Application::loadCompiledIn()
{
	HInputStream compiledInStream = ResourceManager::instance()->getInputStream(L"CompiledIn.json");
	if (compiledInStream.isSet())
		m_jCompiledIn = JSON::parse(compiledInStream);
}

Version
Application::getCurrentVersion()
{
	UTF16 sVersion = m_jCompiledIn[L"version"].as_string();
	return Version(sVersion);
}

Version Application::getLatestVersion()
{
	return m_latestVersion;
}

UTF16 Application::getLatestURL()
{
	return m_sLatestVersionURL;
}

void
Application::loadSettings()
{
	Error error;
	HInputStream settingsFileStream;
	m_settingsFile.readFile(error, settingsFileStream);
	if (error)
		logerr("Unable to read settings file \"%s\" because \"%s\"", m_settingsFile.ptr(), UTF16(error).ptr());

	if (settingsFileStream.isSet())
	{
		m_jSettings = JSON::parse(settingsFileStream);
	}
}

void
Application::saveSettings()
{
	Error error;
	HOutputStream settingsFileStream;
	m_settingsFile.overwriteFile(error, settingsFileStream);
	if (error)
		logerr("Unable to write settings file \"%s\" because \"%s\"", m_settingsFile.ptr(), UTF16(error).ptr());

	if (settingsFileStream.isSet())
	{
		m_jSettings.generate(error, settingsFileStream);
		if (error)
			logerr("Unable to generate settings file \"%s\" because \"%s\"", m_settingsFile.ptr(), UTF16(error).ptr());
	}
}

bool Application::isAutocheckEnabled()
{
	loadSettings();
	return m_jSettings[L"autocheck"].as_bool();
}

class ApplicationCheckVersionListener : public FolderscopeServerRPC::CheckVersionListener
{
public:
	WHApplication m_app;
	ApplicationCheckVersionListener(WHApplication app) : m_app(app) {}

	void checkVersionStart()
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->checkVersionStart();
	}

	virtual void checkVersionFailed()
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->checkVersionFailed();
	}

	virtual void checkVersionSuccess(Version latest, UTF16 url)
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->checkVersionSuccess(latest, url);
	}
};

class ApplicationSubscribeNewsletterListener : public FolderscopeServerRPC::SubscribeNewsletterListener
{
public:
	WHApplication m_app;
	ApplicationSubscribeNewsletterListener(WHApplication app) : m_app(app) {}

	void subscribeNewsletterStart()
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->subscribeNewsletterStart();
	}

	virtual void subscribeNewsletterFailed()
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->subscribeNewsletterFailed();
	}

	virtual void subscribeNewsletterSuccess()
	{
		HApplication h = m_app;
		if (h.isSet())
		   h->subscribeNewsletterSuccess();
	}
};

void Application::checkVersion()
{
	loadSettings();

	UTF16 track = m_jCompiledIn[L"track"].as_string();
	UTF16 install_id = m_jSettings[L"install_id"].as_string();
	UTF16 version = m_jCompiledIn[L"version"].as_string();
	UTF16 email = m_jSettings[L"email"].as_string();

	HApplication app = this;

	m_checkVersionListener = NewObject(ApplicationCheckVersionListener, app);

	FolderscopeServerRPC::checkVersionAsync(m_checkVersionListener, track, install_id, version, email);
}

void Application::checkVersionStart()
{
	loginfo("Performing automatic check for the latest version...");
	m_hWelcomeView->checkVersionStart();
}

void Application::checkVersionSuccess(Version latest, UTF16 sUrl)
{
	m_latestVersion = latest;
	m_sLatestVersionURL = sUrl;

	if (getLatestVersion() == getCurrentVersion())
	{
		loginfo("Check successful: You have the latest version.");
	}
	else if (getLatestVersion() > getCurrentVersion())
	{
		loginfo("Check successful: There is a newer version of Folderscope available. Version %s can be downloaded from %s.", UTF16(latest).ptr(), sUrl.ptr());
	}
	else
	{
		logwarn("It seems that you have a newer version (%s) than the latest version the server reports (%s).  This is most likely an error, please contact support@folderscope.com.", UTF16(getCurrentVersion()).ptr(), UTF16(getLatestVersion()).ptr());
	}

	m_hWelcomeView->checkVersionSuccess(getLatestVersion(), getLatestURL());
}

void Application::checkVersionFailed()
{
	logwarn("Version check failed, most likely because of a temporary network problem.  We will try again later");
	m_hWelcomeView->checkVersionFailed();
}

void Application::enableAutocheck()
{
	loadSettings();
	m_jSettings[L"autocheck"] = JSON(true);
	saveSettings();
}

void Application::disableAutocheck()
{
	loadSettings();
	m_jSettings[L"autocheck"] = JSON(false);
	saveSettings();
}

void Application::subscribeNewsletter(UTF16 email)
{
	loadSettings();
	m_jSettings[L"email"] = JSON(email);
	saveSettings();

	UTF16 install_id = m_jSettings[L"install_id"].as_string();
	UTF16 version = m_jCompiledIn[L"version"].as_string();

	HApplication app = this;

	m_subscribeNewsletterListener = NewObject(ApplicationSubscribeNewsletterListener, app);

	FolderscopeServerRPC::subscribeNewsletterAsync(m_subscribeNewsletterListener, install_id, version, email);
}

void Application::subscribeNewsletterStart()
{
	m_hWelcomeView->subscribeNewsletterStart();
}

void Application::subscribeNewsletterFailed()
{
	m_hWelcomeView->subscribeNewsletterFailed();
}

void Application::subscribeNewsletterSuccess()
{
	m_hWelcomeView->subscribeNewsletterSuccess();
}

void Application::upgrade()
{
	Error error;
	OperatingSystem::open(error, getLatestURL());
	if (error)
		logerr("Unable to open \"%s\" because \"%s\"", getLatestURL().ptr(), UTF16(error).ptr());
}

UTF16 Application::getEmailAddress()
{
	loadSettings();
	return m_jSettings[L"email"].as_string();
}

void Application::tasksAvailable()
{
	if (m_hMainWindow.isSet())
	{
		m_hMainWindow->tasksAvailable();
	}
}
