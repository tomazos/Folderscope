// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#include "Stream/FileSize.h"

Ascii FileSize::getShortString()
{
	if (m_iSize == 0)
		return "0";

	Ascii s1 = Ascii::format("%llu", m_iSize);

	if (s1.length() < 4)
		return s1 + "  ";

	int iUnit = 0;
	for (int i = s1.length(); i > 3; i -= 3)
		iUnit++;

	int iDigitsAfter = iUnit * 3;
	int iDigitsBefore = s1.length() - iDigitsAfter;

	Ascii sb = s1.prefix(iDigitsBefore);
	Ascii sa = s1.suffix(iDigitsAfter);

	Ascii s2 = (sb + "." + sa).prefix(4);

	if (s2.endsWith("."))
		s2 = s2.prefix(3);

	switch (iUnit)
	{
	case 1: return s2 + " k";
	case 2: return s2 + " M";
	case 3: return s2 + " G";
	case 4: return s2 + " T";
	case 5: return s2 + " P";
	case 6: return s2 + " E";
	case 7: return s2 + " Z";
	case 8: return s2 + " Y";
	default: Check(); return s2 + " ?";
	}
}

Ascii FileSize::getLongString()
{
	if (m_iSize == 0)
		return "0";

	Ascii s1 = Ascii::format("%llu", m_iSize);

	Ascii s2 = "";
	while (s1.length() > 3)
	{
		s2 = Ascii(",") + s1.suffix(3) + s2;
		s1 = s1.prefix(s1.length() - 3);
	}

	s2 = s1 + s2;
	return s2;
}
