// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderscopeServerRPC.h"

#include "TaskTracker.h"

bool FolderscopeServerRPC::checkVersionSync(UTF16 track, UTF16 install_id, Version version, UTF16 email, Version& latest, UTF16& url)
{
	JSON request;
	request[L"function"] = JSON(L"checkVersion");
	request[L"params"][L"track"] = JSON(track);
	request[L"params"][L"install_id"] = JSON(install_id);
	request[L"params"][L"version"] = JSON(UTF16(version));
	request[L"params"][L"email"] = JSON(UTF16(email));

	Error error;
	JSON response = HTTP::rpc(error, "http://rpc.folderscope.com", request);

	if (response.isNull() || error)
		return false;

	latest = Version(response[L"latest"].as_string());
	url = response[L"url"].as_string();

	return true;
}

void FolderscopeServerRPC::checkVersionAsync(WHCheckVersionListener hListener, UTF16 track, UTF16 install_id, Version version, UTF16 email)
{
	HTask task = buildFunctionTask(FolderscopeServerRPC::checkVersionTask, hListener, track, install_id, version, email);
	TaskTracker::instance()->add(task, TaskTracker::eOwnThread);
}

void FolderscopeServerRPC::checkVersionTask(WHCheckVersionListener hListener, UTF16 track, UTF16 install_id, Version version, UTF16 email)
{
	{
		HCheckVersionListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::CheckVersionListener::checkVersionStart), TaskTracker::eMainThread);
	}

	Version latest;
	UTF16 url;
	if (!checkVersionSync(track, install_id, version, email, latest, url))
	{
		HCheckVersionListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::CheckVersionListener::checkVersionFailed), TaskTracker::eMainThread);
		return;
	}

	{
		HCheckVersionListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::CheckVersionListener::checkVersionSuccess, latest, url), TaskTracker::eMainThread);
	}
}

bool FolderscopeServerRPC::subscribeNewsletterSync(UTF16 install_id, Version version, UTF16 email)
{
	JSON request;
	request[L"function"] = JSON(L"subscribeNewsletter");
	request[L"params"][L"install_id"] = JSON(install_id);
	request[L"params"][L"version"] = JSON(UTF16(version));
	request[L"params"][L"email"] = JSON(UTF16(email));

	Error error;
	JSON response = HTTP::rpc(error, "http://rpc.folderscope.com", request);

	if (error)
		return false;

	return response.as_string() == L"ok";
}

void FolderscopeServerRPC::subscribeNewsletterAsync(WHSubscribeNewsletterListener hListener, UTF16 install_id, Version version, UTF16 email)
{
	HTask task = buildFunctionTask(FolderscopeServerRPC::subscribeNewsletterTask, hListener, install_id, version, email);
	TaskTracker::instance()->add(task, TaskTracker::eOwnThread);
}

void FolderscopeServerRPC::subscribeNewsletterTask(WHSubscribeNewsletterListener hListener, UTF16 install_id, Version version, UTF16 email)
{
	{
		HSubscribeNewsletterListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::SubscribeNewsletterListener::subscribeNewsletterStart), TaskTracker::eMainThread);
	}

	if (!subscribeNewsletterSync(install_id, version, email))
	{
		HSubscribeNewsletterListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::SubscribeNewsletterListener::subscribeNewsletterFailed), TaskTracker::eMainThread);
		return;
	}

	{
		HSubscribeNewsletterListener l = hListener;
		TaskTracker::instance()->add(buildMethodTask(l.ptr(), &FolderscopeServerRPC::SubscribeNewsletterListener::subscribeNewsletterSuccess), TaskTracker::eMainThread);
	}
}
