// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Runtime/ResourceManager.h"

ResourceManager* ResourceManager::instance()
{
	GlobalSync;

	static ResourceManager* p(NULL);
	if (!p)
	{
		p = NewObject(ResourceManager);
		ResourceManager::setup();
	}

	return p;
}

HInputStream ResourceManager::getInputStream(const UTF16& sPath)
{
	Resource r = get(sPath);
	return NewObject(MemoryInputStream, r.ptr(), r.length());
}
	
void ResourceManager::writeToFile(Error& error, const UTF16& sRezPath, const Path& destFile)
{
	HInputStream in = getInputStream(sRezPath);
	HOutputStream out = NewObject(FileOutputStream, error, destFile, FileOutputStream::eOverwrite);

	if (error)
		return;

	in->pipe(error, out);

	if (error)
		return;
}

void ResourceManager::set(const UTF16& sPath, const void* pData, Size iNumBytes)
{
	m_resources.add(sPath, Resource(pData, iNumBytes));
}

Resource ResourceManager::get(const UTF16& sPath)
{
	return m_resources.get(sPath);
}
