#pragma once
#include "NewsCommonObjects.h"

class DownloadingNewsThread
{
public:
	void operator()(NewsCommonObjects& common);
};
