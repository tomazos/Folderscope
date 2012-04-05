// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Runtime.h"

class FolderscopeServerRPC
{
public:
	static bool checkVersionSync(UTF16 track, UTF16 install_id, Version version, UTF16 email, Version& latest, UTF16& url);

	DeclWeakHandleable(CheckVersionListener);
	class CheckVersionListener : public WeakHandleable
	{
	public:

		virtual void checkVersionStart() = 0;
		virtual void checkVersionFailed() = 0;
		virtual void checkVersionSuccess(Version latest, UTF16 url) = 0;
	};

	static void checkVersionAsync(WHCheckVersionListener hListener, UTF16 track, UTF16 install_id, Version version, UTF16 email);
	
	static void checkVersionTask(WHCheckVersionListener hListener, UTF16 track, UTF16 install_id, Version version, UTF16 email);

	static bool subscribeNewsletterSync(UTF16 install_id, Version version, UTF16 email);

	DeclWeakHandleable(SubscribeNewsletterListener);
	class SubscribeNewsletterListener : public WeakHandleable
	{
		friend class FolderscopeServerRPC;

		virtual void subscribeNewsletterStart() = 0;
		virtual void subscribeNewsletterFailed() = 0;
		virtual void subscribeNewsletterSuccess() = 0;
	};

	static void subscribeNewsletterAsync(WHSubscribeNewsletterListener, UTF16 install_id, Version version, UTF16 email);

	static void subscribeNewsletterTask(WHSubscribeNewsletterListener hListener, UTF16 install_id, Version version, UTF16 email);

};

