#pragma once
#include "NewsCommonObjects.h"

class DrawNewsWindowThread
{
public:
	void operator()(NewsCommonObjects& common);
};

