#include "DrawingObject.h"

DrawingObject::DrawingObject()
	: m_core(NULL)
{}

DrawingObject::DrawingObject(const DrawingObject& that)
	: m_core(that.m_core)
{}

DrawingObject& DrawingObject::operator=(const DrawingObject& that)
{
	m_core = that.m_core; return *this;
}

DrawingObject::DrawingObject(HGDIOBJ gdiobj, EDestroyHandle eDestroyHandle)
	: m_core(new Core(gdiobj, eDestroyHandle))
{}

DrawingObject::Core::Core(HGDIOBJ gdiobj, EDestroyHandle eDestroyHandle)
	: m_gdiobj(gdiobj)
	, m_eDestroyHandle(eDestroyHandle)
{}

DrawingObject::Core::~Core()
{
	if (m_gdiobj != NULL && m_eDestroyHandle == eDoDestroy)
	{
		BOOL res = DeleteObject(m_gdiobj);
		Check (res != 0);
	}
}
