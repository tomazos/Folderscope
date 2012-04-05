// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"

class FIItem;

enum FICriteria
{
	eFullpath,
	eBasename,
	eSize,
	eType,
	eLink,
	eContent,
	eCreated,
	eModified,
	eMarked
};

enum FICention
{
	eAscending,
	eDescending,
	eNone
};

class FISingleComparitor
{
public:
	FISingleComparitor() : m_crit(eFullpath), m_cent(eNone) {}

	FISingleComparitor(FICriteria crit, FICention cent) : m_crit(crit), m_cent(cent) {}

	SInt64 operator()(FIItem* a, FIItem* b);

	FICriteria m_crit;
	FICention m_cent;
};

class FIMultiComparitor
{
public:
	void add(FICriteria crit, FICention cent)
	{
		for (int i = 0; i < m_comps.size(); i++)
		{
			if (m_comps[i].m_crit == crit)
			{
				m_comps.del(i);
				break;
			}
		}
		if (cent != eNone)
			m_comps.insert(0, FISingleComparitor(crit,cent));
	}

	SInt64 operator()(FIItem* a, FIItem* b)
	{
		for (int i = 0; i < m_comps.size(); i++)
		{
			SInt64 res = m_comps[i](a,b);
			if (res != 0)
				return res;
		}
		return 0;
	}

	void status(FICriteria crit, int& iPriority, FICention& cent)
	{
		for (int i = 0; i < m_comps.size(); i++)
		{
			if (m_comps[i].m_crit == crit)
			{
				iPriority = i+1;
				cent = m_comps[i].m_cent;
				return;
			}
		}
		iPriority = 0;
		cent = eNone;
	}

private:
	Array<FISingleComparitor> m_comps;
};
