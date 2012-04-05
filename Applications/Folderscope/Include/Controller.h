// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

#include "FolderImage.h"

class Controller
{
public:
	static Controller* get();

	class Listener
	{
	public:
		virtual void Controller_onAddImage(const HFolderImage&) = 0;
		virtual void Controller_onDelImage(const HFolderImage&) = 0;
		virtual void Controller_onFocusImage(const HFolderImage&) = 0;
		virtual void Controller_onBlurImage(const HFolderImage&) = 0;
	};

	void addListener(Listener* pListener) { m_listeners += pListener; }
	void removeListener(Listener* pListener) { m_listeners -= pListener; }

	void open(Error&, const Path& target);

	void close();
	void refresh();
	void import();
	void quit();
	void sort(FICriteria, FICention);
	void deep_scan();
    void mark_all();
    void mark_none();
    void mark_invert();
    void mark_duplicates();
    void mark_match_path();
    void mark_match_content();
    void filter_undo();
    void filter_redo();
    void filter_marked();
    void delete_();
    void rename();
    void copy();
    void external_copy();
    void external_move();
    void changes_remove();
    void changes_commit();

	void addImage(const HFolderImage&);
	void delImage(const HFolderImage&);
	void focusImage(const HFolderImage&);
	void blurImage(const HFolderImage&);

private:

	void Controller_onFocusImage(const HFolderImage& image)
	{
		TreeSet<Listener*>::Iterator it(m_listeners);
		Listener* p;
		while (it(p))
			p->Controller_onFocusImage(image);
	}

	void Controller_onBlurImage(const HFolderImage& image)
	{
		TreeSet<Listener*>::Iterator it(m_listeners);
		Listener* p;
		while (it(p))
			p->Controller_onBlurImage(image);
	}

	void Controller_onAddImage(const HFolderImage& image)
	{
		TreeSet<Listener*>::Iterator it(m_listeners);
		Listener* p;
		while (it(p))
			p->Controller_onAddImage(image);
	}

	void Controller_onDelImage(const HFolderImage& image)
	{
		TreeSet<Listener*>::Iterator it(m_listeners);
		Listener* p;
		while (it(p))
			p->Controller_onDelImage(image);
	}

	TreeSet<Listener*> m_listeners;
	TreeSet<HFolderImage> m_images;
	HFolderImage m_focus;
};
