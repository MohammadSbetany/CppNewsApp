// DownloadingNewsThread.cpp : this file init the news relvent objects and also load the data to it

#include "DownloadingNewsThread.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>


void DownloadingNewsThread::operator()(NewsCommonObjects& common) //downloading articles from News API site
{
    httplib::Client cli("https://newsapi.org");

	auto res = cli.Get("/v2/everything?q=apple&from=2024-08-27&to=2024-08-27&sortBy=popularity&apiKey=aab1e14f50cf408ba59303839bad5b11");
	if (res->status) {
		auto json_result = nlohmann::json::parse(res->body);
		std::filesystem::path filePath = "NewsInJsonFormat.json";
		std::fstream file(filePath,std::ios::out | std::ios::trunc);// creating a file that have all the news detailes in json format
		if (file.is_open()) {
			file << res->body;
			file.close();
		}
		else 
			std::cout << "Error: Unable to create the news file.\n";

		for (auto& articleContent : json_result["articles"]) { //init article objects with its suitable values
			Article article;
			article.content = articleContent;
			article.isPreferred = false;
			article.showFullDetailesWindow = false;
			common.allArticles.push_back(article);
		}

			common.isDataReady = true; //notifying initNewsWindow thread to run because the data have been downloaded 
			common.isDataReady.notify_one();
	}
}