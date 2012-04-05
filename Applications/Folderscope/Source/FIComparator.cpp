// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "FIComparator.h"

#include "FIItem.h"

static inline SInt64 comp_size(FIItem* a, FIItem* b)
{
	UInt64 asz = a->getSize();
	UInt64 bsz = b->getSize();
	if (asz == bsz)
		return 0;
	else if (asz > bsz)
		return 1;
	else
		return -1;
}

static inline SInt64 comp_fullpath(FIItem* a, FIItem* b)
{
	FIItem::Components& ac = (*(a->getComponents().ptr()));
	FIItem::Components& bc = (*(b->getComponents().ptr()));

	int iMin = (ac.size() < bc.size() ? ac.size() : bc.size());

	for (int i = 0; i < iMin; i++)
	{
		SInt64 iRes = compareUTF16CaseInsensetive(ac[i],bc[i]);
		if (iRes != 0)
			return iRes;
	}
	return ac.size() - bc.size();
}

static inline SInt64 comp_basename(FIItem* a, FIItem* b)
{
	return compareUTF16CaseInsensetive(a->getBasename(), b->getBasename());
}

static inline SInt64 comp_type(FIItem* a, FIItem* b)
{
	return compareUTF16CaseInsensetive(a->getExt(), b->getExt());
}

static inline SInt64 comp_link(FIItem* a, FIItem* b)
{
	return DefaultComparator<UInt32>::comparef(a->getLink(), b->getLink());
}

static inline SInt64 comp_content(FIItem* a, FIItem* b)
{
	return DefaultComparator<UInt32>::comparef(a->getContent(), b->getContent());
}

static inline SInt64 comp_created(FIItem* a, FIItem* b)
{
	return TimePoint::comparef(a->getCreated(), b->getCreated());
}

static inline SInt64 comp_modified(FIItem* a, FIItem* b)
{
	return TimePoint::comparef(a->getModified(), b->getModified());
}

static inline SInt64 comp_marked(FIItem* a, FIItem* b)
{
	return 0; // TODO
}

SInt64 FISingleComparitor::operator()(FIItem* a, FIItem* b)
{
	SInt64 res = 0;
	switch (m_crit)
	{
	case eFullpath: res = comp_fullpath(a,b); break;
	case eSize: res = comp_size(a,b); break;
	case eBasename: res = comp_basename(a,b); break;
	case eType: res = comp_type(a,b); break;
	case eLink: res = comp_link(a,b); break;
	case eContent: res = comp_content(a,b); break;
	case eCreated: res = comp_created(a,b); break;
	case eModified: res = comp_modified(a,b); break;
	case eMarked: res = comp_marked(a,b); break;
	default: Check();
	}

	if (m_cent == eDescending)
		res = -res;

	return res;
}
