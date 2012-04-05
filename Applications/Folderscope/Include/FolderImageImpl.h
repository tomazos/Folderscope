// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "FIItem.h"
#include "FIFile.h"
#include "FIDir.h"

#include "SurfaceScanner.h"
#include "DeepScanner.h"

#include "FolderImageMode.h"
#include "FolderImage.h"

#include "TaskRepeater.h"
#include "FIDatabase.h"

class FolderImageImpl : public FolderImage, public SurfaceScanner::Master, DeepScanner::Master
{
public:
	Lock m_lock;
	FIDatabase m_db;

	Array<FIDir*> m_pFIDirStack;

	FIMultiComparitor m_comp;

	HInputStream m_hLoadStream;

	virtual Path getRoot() { return m_db.m_root; }
	virtual UTF16 getShortRoot() { return m_db.m_root.basename(); }

	// CONSTRUCTION
	FolderImageImpl();
	void setRoot(const Path& root);
	void setLoadStream(HInputStream hLoadStream);
	virtual ~FolderImageImpl();

	// LISTENER
	TreeSet<Listener*> m_listeners;
	virtual void addListener(Listener* pListener);
	virtual void removeListener(Listener* pListener);

	template<class F>
	void notifyListeners(F f);
	template<class F, class P1>
	void notifyListeners(F f, P1 p1);
	template<class F, class P1, class P2>
	void notifyListeners(F f, P1 p1, P2 p2);

	void FolderImage_enteringState(EState eState);
	void FolderImage_leavingState(EState eState);
	void FolderImage_progress(const ProgressReport&);
	void FolderImage_info(UTF16 sInfo);
	void FolderImage_warning(UTF16 sError);
	void FolderImage_fatal(UTF16 sError);
	void FolderImage_quickJobStart();
	void FolderImage_quickJobEnd();

	// MODE
	EState m_eState;
	FolderImageMode* m_mode;
	template<class M>
	void changeMode(EState eState);

	// SURFACE SCAN
	HSurfaceScanner m_surfaceScanner;
	void surfaceScan(bool& bComplete);
	virtual FIItem* SurfaceScanner_startFolder(const Path& folder, Array<UTF16>& relative);
	virtual void SurfaceScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage);
	virtual void SurfaceScanner_processFile(Error& error, const Path& file, const Array<UTF16>& relative);
	virtual void SurfaceScanner_progress(const ProgressReport& timer);
	virtual void SurfaceScanner_endFolder(FIItem*);

	// DEEP SCAN
	HDeepScanner m_deepScanner;
	void deepScan(bool& bComplete);
	virtual void DeepScanner_skipWarning(const UTF16& sAbsolutePath, const UTF16& sErrorMessage);
	virtual void DeepScanner_progress(const ProgressReport& timer);

	// TASKS
	HTaskThread m_taskThread;
	void queueTask(HTask task);
	template <class F>
	void deferTask(F f);
	template <class F, class P1>
	void deferTask(F f, P1 p1);
	template <class F, class P1, class P2>
	void deferTask(F f, P1 p1, P2 p2);

	// MODE CALLBACKS
	virtual void a_save(HOutputStream hOutputStream);
	virtual void save(HOutputStream hOutputStream);
	virtual void a_work();
	virtual void work();
	virtual void a_start();
    virtual void start();
	virtual void a_pause();
    virtual void pause();
	virtual void a_refresh();
    virtual void refresh();
	virtual void a_stop();
    virtual void stop();
	virtual void a_shutdown();
	virtual void shutdown();
	
	virtual Size getNoOfItems();
	virtual FIItem* getItem(Size iIndex);

	virtual void sortStatus(FICriteria, int&, FICention&);
	virtual void a_sort(FICriteria, FICention);
    virtual void sort(FICriteria, FICention);

	virtual void deepScan();
	virtual void a_deepScan();

	virtual void toggleMarkItem(Size iIndex);
	virtual void a_markAll();
    virtual void markAll();
	virtual void a_markNone();
    virtual void markNone();
	virtual void a_markInvert();
    virtual void markInvert();
	virtual void a_markDuplicates();
    virtual void markDuplicates();
	virtual void a_markMatchPath();
    virtual void markMatchPath();
	virtual void a_markMatchContent();
    virtual void markMatchContent();

	virtual void a_deleteMarked();
    virtual void deleteMarked();
	virtual void a_renameMarked(UTF16 sSource, UTF16 sDestination);
	virtual void renameMarked(UTF16 sSource, UTF16 sDestination);
	virtual void a_copyMarked(UTF16 sSource, UTF16 sDestination);
	virtual void copyMarked(UTF16 sSource, UTF16 sDestination);
	virtual void a_externalCopy(UTF16 sAbsoluteExternalRoot);
	virtual void externalCopy(UTF16 sAbsoluteExternalRoot);
	virtual void a_externalMove(UTF16 sAbsoluteExternalRoot);
	virtual void externalMove(UTF16 sAbsoluteExternalRoot);
	virtual bool areChangesPending();
	virtual Size getNoOfChanges();
	virtual FIChange* getChange(Size iIndex);
	virtual void removeChange(FIChange* fiChange);
	virtual void a_commitChanges();
    virtual void commitChanges();
};

template<class F>
void FolderImageImpl::notifyListeners(F f)
{
	Synchronizer sync(m_lock);

	TreeSet<Listener*>::Iterator it(m_listeners);
	Array<Listener*> pListeners;
	Listener* pListener;
	while (it(pListener))
		pListeners.push(pListener);
	for (int i = 0; i < pListeners.size(); i++)
		(pListeners[i]->*f)();
}

template<class F, class P1>
void FolderImageImpl::notifyListeners(F f, P1 p1)
{
	Synchronizer sync(m_lock);

	TreeSet<Listener*>::Iterator it(m_listeners);
	Array<Listener*> pListeners;
	Listener* pListener;
	while (it(pListener))
		pListeners.push(pListener);
	for (int i = 0; i < pListeners.size(); i++)
		(pListeners[i]->*f)(p1);
}

template<class F, class P1, class P2>
void FolderImageImpl::notifyListeners(F f, P1 p1, P2 p2)
{
	Synchronizer sync(m_lock);

	TreeSet<Listener*>::Iterator it(m_listeners);
	Array<Listener*> pListeners;
	Listener* pListener;
	while (it(pListener))
		pListeners.push(pListener);
	for (int i = 0; i < pListeners.size(); i++)
		(pListeners[i]->*f)(p1,p2);
}

template<class M>
void FolderImageImpl::changeMode(EState eState)
{
	FolderImage_leavingState(m_eState);
	FolderImageMode* oldMode = m_mode;
	m_mode = NewObject(M, this);
	DelObject(oldMode);
	m_eState = eState;
	FolderImage_enteringState(m_eState);
};

template <class F>
void FolderImageImpl::deferTask(F f)
{
	HTask h = buildMethodTask(this, f);
	queueTask(h);
}

template <class F, class P1>
void FolderImageImpl::deferTask(F f, P1 p1)
{
	HTask h = buildMethodTask(this, f, p1);
	queueTask(h);
}

template <class F, class P1, class P2>
void FolderImageImpl::deferTask(F f, P1 p1, P2 p2)
{
	HTask h = buildMethodTask(this, f, p1, p2);
	queueTask(h);
}
