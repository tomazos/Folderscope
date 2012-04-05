// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Runtime/Resource.h"

class ResourceManager
{
public:

	static ResourceManager* instance();

	Resource get(const UTF16& sPath);
	void set(const UTF16& sPath, const void* pData, Size iNumBytes);

	HInputStream getInputStream(const UTF16& sPath);
	
	void writeToFile(Error& error, const UTF16& sRezPath, const Path& destFile);

	static void setup();

private:
	ResourceManager() {}

	TreeMap<UTF16, Resource> m_resources;
};
