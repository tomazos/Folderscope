// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

struct FolderSuggestion
{
	Path path;
	UTF16 notes;
};

typedef LinkedList<FolderSuggestion> FolderSuggestionList;
typedef Handle<FolderSuggestionList> HFolderSuggestionList;

DeclHandleable(FolderSuggestionProvider);
class FolderSuggestionProvider : public Thread
{
public:
	FolderSuggestionProvider()
		: m_bReset(false)
		, m_bShutdown(false)
	{}

	void search(const UTF16& sPrefix, int iSuggestionsRequired);

	void shutdown();

	virtual void run();

	class Listener
	{
	public:
		virtual void notifySuggestions(HFolderSuggestionList) = 0;
	};

	void addListener(Listener* pListener);

	void removeListener(Listener* pListener);

private:

	void listPathCompletions(const UTF16& sPrefix, FolderSuggestionList& suggestions, int iSuggestionsRequired);

	void recursePathCompletions(int& iSuggestionsRemaining, FolderSuggestionList& suggestions);

	TreeSet<Listener*> m_pListeners;

	Turnstile m_work;
	bool m_bReset, m_bShutdown;
	Lock m_lock;
	UTF16 m_sPrefix;
	int m_iSuggestionsRequired;
};
