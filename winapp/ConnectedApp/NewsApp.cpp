// NewsApp.cpp : This file contains the 'main' function,The Program execution begins and ends here.

#include <iostream>
#include <thread>
#include "NewsCommonObjects.h"
#include "initNewsWindow.h"
#include "DownloadingNewsThread.h"

int main() {
    NewsCommonObjects common;
    DrawNewsWindowThread draw;
    DownloadingNewsThread down;
    auto draw_th = std::jthread([&] {draw(common); }); //drawing the windows thread
    auto down_th = std::jthread([&] {down(common); }); //downloading the news data to common
    std::cout << "The program started" << std::endl;
    down_th.join();
    draw_th.join();
    std::cout << "The program is ending...." << std::endl;
}