// (C) 2009 Tomazos-Jager Software Labs.  All rights reserved.
#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream/OutputStream.h"

#include <vector>

/* forward decl */ class JSON;
DeclHandleable(JSONArrayCore);

class JSONArray
{
	friend class JSON;

	std::vector<JSON> m_items;

	Size size() const;
	JSON& operator[](Size iSize);

	void push(const JSON&);
	JSON pop();

	void generate(Error&, const HOutputStream&) const;

	void as_string(Error&, const HOutputStream&) const;
	JSON as_number() const;
	JSON as_object() const;

	static SInt64 comparef(const JSONArray& a, const JSONArray& b);
};
