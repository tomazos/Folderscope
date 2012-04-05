#pragma once
#include "global.h"

#include "Basics.h"

class DrawingObject
{
public:
	DrawingObject();
	DrawingObject(const DrawingObject&);

	DrawingObject& operator=(const DrawingObject& that);

protected:
	enum EDestroyHandle
	{
		eDoDestroy,
		eDontDestroy
	};

	DrawingObject(HGDIOBJ gdiobj, EDestroyHandle);

	class Core : public Handleable
	{
	public:
		Core(HGDIOBJ gdiobj, EDestroyHandle);
		~Core();
		HGDIOBJ m_gdiobj;
		EDestroyHandle m_eDestroyHandle;
	};

	Handle<Core> m_core;
};

