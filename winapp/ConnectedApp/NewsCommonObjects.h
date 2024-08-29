#pragma once
#include <atomic>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

struct Article {
	bool isPreferred = false;
	bool showFullDetailesWindow = false;
	nlohmann::json content;
};

struct NewsCommonObjects {// it represent the objects that should be in the two threads (download thread and draw thread)
	std::atomic<bool> isDataReady = false;
	std::vector <Article> allArticles;
};