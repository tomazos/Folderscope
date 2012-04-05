#pragma once
#include "global.h"

#include "Basics.h"
#include "Stream.h"

class HTTP
{
public:
	static JSON rpc(Error&, Ascii sUrl, const JSON& jRequest);
};
