// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FolderImage.h"

#include "FolderImageImpl.h"

HFolderImage FolderImage::create(const Path& root)
{
	FolderImageImpl* p = NewObject(FolderImageImpl);
	p->setRoot(root);
	return p;
}

HFolderImage load(HInputStream is)
{
	FolderImageImpl* p = NewObject(FolderImageImpl);
	p->setLoadStream(is);
	return p;
}
