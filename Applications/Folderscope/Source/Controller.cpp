// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Controller.h"

#include "Basics.h"
#include "Runtime.h"

Controller* Controller::get()
{
	static Controller* p(NULL);
	if (p == NULL)
		p = NewObject(Controller);
	return p;
}

void Controller::open(Error& error, const Path& targetRoot)
{
	bool bExists = targetRoot.exists(error);

	if (error)
		return;

	if (!bExists)
	{
		targetRoot.vivDirForce(error);

		if (error)
			return;
	}

	bool bIsDir = targetRoot.isDir(error);

	if (error)
		return;

	if (!bIsDir)
	{
		OperatingSystem::open(error, targetRoot);

		if (error)
			return;

		return;
	}

	TreeSet<HFolderImage>::Iterator it(m_images);
	HFolderImage targetImage;
	HFolderImage itImage;
	while (it(itImage))
	{
		Path itRoot = itImage->getRoot();
		if (itRoot == targetRoot)
		{
			targetImage = itImage;
			break;
		}
	}
		
	if (targetImage.isNull())
	{
		targetImage = FolderImage::create(targetRoot);

		addImage(targetImage);
		targetImage->start();
	}
	
	focusImage(targetImage);
};

void Controller::addImage(const HFolderImage& image)
{
	if (!m_images[image])
	{
		m_images += image;
		Controller_onAddImage(image);
	}
}

void Controller::delImage(const HFolderImage& image)
{
	if (m_focus == image)
		blurImage(image);

	if (m_images[image])
	{
		m_images -= image;
		Controller_onDelImage(image);
	}
}

void Controller::focusImage(const HFolderImage& image)
{
	if (m_images[image] && m_focus != image)
	{
		m_focus = image;
		Controller_onFocusImage(image);
	}
}

void Controller::blurImage(const HFolderImage& image)
{
	if (m_images[image] && m_focus == image)
	{
		m_focus.setNull();
		Controller_onBlurImage(image);
	}
}

void Controller::close()
{
	if (m_focus.isSet())
		m_focus->shutdown();
}

void Controller::refresh()
{
    if (m_focus.isSet())
        m_focus->refresh();
}

void Controller::import()
{
    if (m_focus.isSet())
		loginfo("TODO");
}

void Controller::sort(FICriteria crit, FICention cent)
{
    if (m_focus.isSet())
        m_focus->sort(crit, cent);
}

void Controller::deep_scan()
{
    if (m_focus.isSet())
        m_focus->deepScan();
}

void Controller::mark_all()
{
    if (m_focus.isSet())
        m_focus->markAll();
}

void Controller::mark_none()
{
    if (m_focus.isSet())
        m_focus->markNone();
}

void Controller::mark_invert()
{
    if (m_focus.isSet())
        m_focus->markInvert();
}

void Controller::mark_duplicates()
{
    if (m_focus.isSet())
        m_focus->markDuplicates();
}

void Controller::mark_match_path()
{
    if (m_focus.isSet())
        m_focus->markMatchPath();
}

void Controller::mark_match_content()
{
    if (m_focus.isSet())
        m_focus->markMatchContent();
}

void Controller::delete_()
{
    if (m_focus.isSet())
        m_focus->deleteMarked();
}

void Controller::rename()
{
    if (m_focus.isSet())
		loginfo("TODO");
}

void Controller::copy()
{
    if (m_focus.isSet())
		loginfo("TODO");
}

void Controller::external_copy()
{
    if (m_focus.isSet())
		loginfo("TODO");
}
void Controller::external_move()
{
    if (m_focus.isSet())
		loginfo("TODO");
}

void Controller::changes_remove()
{
	loginfo("TODO");
}

void Controller::changes_commit()
{
    if (m_focus.isSet())
        m_focus->commitChanges();
}
