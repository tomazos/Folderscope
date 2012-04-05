#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

class FolderScanerState
{
public:
	HUTF16List subpaths;
	UTF16List::Iterator subpath_it;
	UTF16 relative;
	UTF16 absolute;
	FIItem* ancestor;
	Real64 scanunit;
}
