// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream/OutputStream.h"

/* forward decl */ class JSON;

class JSONObject
{
	friend class JSON;

	JSONObject();
	JSONObject(const JSONObject&);

	Size size() const;
	JSON& operator[](const UTF16& sKey);
	void del(const UTF16& sKey);

	void generate(Error&, const HOutputStream&) const;

	JSON as_number() const;
	void as_string(Error&, const HOutputStream&) const;
	JSON as_array() const;

	DeclHandleable(Core);
	class Core : public Handleable
	{
	public:
		TreeMap<UTF16, JSON> m_tree;
	};

	HCore m_core;
};

