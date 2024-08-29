// initNewsWindow.cpp : this file create the news window and also all relvented windows such as preferred title window and full detailes title

#include "../../shared/ImGuiSrc/imgui.h"
#include "initNewsWindow.h"
#include "GuiMain.h"
#include <iostream>
#include <windows.h>

bool showPreferredTitlesWindow = false; //seting prefered titles window to not been showed
bool showArticlesWindow = true;//seting titles window to been shown by default

void OpenURL(const char* url) {//function to open URL in website
	int len = MultiByteToWideChar(CP_ACP, 0, url, -1, NULL, 0);//converting url from char* to wide string
	wchar_t* wurl = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, url, -1, wurl, len);
	ShellExecute(0, 0, wurl, 0, 0, SW_SHOW);//converting url to wide string because this function can only get wide string parameter in its third parameter
	delete[] wurl;
}

void CenteredScaledText(const char* text, float scale) {// putting the text int the middle of the line and also setting its size
	float windowWidth = ImGui::GetWindowSize().x;
	ImGui::SetWindowFontScale(scale);

	float textWidth = ImGui::CalcTextSize(text).x;
	float textPosX = (windowWidth - textWidth * scale) * 0.5f;

	if (textPosX > 0.0f) 
		ImGui::SetCursorPosX(textPosX);

	ImGui::Text("%s", text);
	ImGui::SetWindowFontScale(1.0f);
}



void SetCustomTheme() { // Function to set the custom theme
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Black background
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));      // White text
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));    // Dark Gray button
}

void ResetTheme() { // Function to reset the theme back to the original state
	ImGui::PopStyleColor(3);
}

void drawFullDetailesWindow(Article& article,int articleNum) {// creating window that have the full title deatiles
	article.showFullDetailesWindow = true;
	std::string howManySpaces = ""; //in this string we put number of spaces that is equal to the
	//article number to give each window different ID because if they had the same label (each article) then
	//all of them will be shown in the same window (the deatiles of each article will be shown in one window and not separated windows)
	for (int i = 0; i < articleNum; i++)
		howManySpaces += " ";

	ImGui::Begin(("Article detailes" + howManySpaces).c_str());
	std::string title = article.content["title"];
	ImGui::SetWindowFontScale(1.25f);	// Scale the text size by a factor of 1.5x
	ImGui::Text(("Title: " + title).c_str());
	ImGui::SetWindowFontScale(1.0f);// Reset back to normal size for the rest of the UI

	nlohmann::json source = article.content["source"];
	std::string sourceName = source["name"];
	ImGui::Text(("Source: " + sourceName).c_str());

	std::string author = article.content["author"];
	ImGui::Text(("Author: " + author).c_str());

	std::string publishedDate = article.content["publishedAt"];
	ImGui::Text(("published at: " + publishedDate).c_str());

	std::string URLLink = article.content["url"];
	ImGui::Text("URL link: ");

	ImGui::SameLine();
	if (ImGui::Button(URLLink.c_str())) 
		OpenURL(URLLink.c_str());
	
	if (ImGui::Button("Close"))
		article.showFullDetailesWindow = false;

	ImGui::End();
}

void drawPreferredTitlesWindow(NewsCommonObjects* common) {// creating window that have all the prefered titles, you should click the button to open it
	showPreferredTitlesWindow = true;

	ImGui::Begin("Preferred titles");
	ImGui::Text(" ");

	int articleNum = 0;
	for (auto& article : common->allArticles) {
		articleNum++;
		if (article.isPreferred) {
			std::string title = article.content["title"];
			ImGui::Text(title.c_str());

			ImGui::PushID(articleNum);// in each pushID  function we will put differnet equation to make sure that no id will be repated
			if (ImGui::Button("show detailes"))
				article.showFullDetailesWindow = true;
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID(articleNum + common->allArticles.size());
			if (ImGui::Button("remove from preferred"))
				article.isPreferred = !article.isPreferred;
			ImGui::PopID();

			ImGui::Text(" ");//go down one line
		}
	}
	if (ImGui::Button("Close"))
		showPreferredTitlesWindow = false;

	ImGui::End();   
}

void drawNewsWindow(NewsCommonObjects* common) {// creating a window that have all the news titles (main window) created by default
	ImGui::Begin("News");
	ImGui::Text(" ");//go down one line
	ImGui::Text("Search: ");

	ImGui::SameLine();
	static char buff[200];
	ImGui::InputText(" ", buff, sizeof(buff));

	ImGui::SameLine();
	if ((ImGui::Button("Open preferred titles list") || showPreferredTitlesWindow) && common->isDataReady) {
		ImGui::End();
		drawPreferredTitlesWindow(common);
		ImGui::Begin("News");
	}

	if (common->isDataReady) {
		ImGui::Text(" ");//go down one line
		CenteredScaledText("News", 1.75);
		ImGui::Text(" ");//go down one line

		int articleNum = 0;
		for (auto& article : common->allArticles) {
			articleNum++;
			std::string title = article.content["title"];
			if (title.find(buff) == std::string::npos)// if the searched string is not a part of this article title
				continue;

			else {
				ImGui::SetWindowFontScale(1.25f);	// Scale the text size by a factor of 1.5x
				ImGui::Text(title.c_str());
				ImGui::SetWindowFontScale(1.0f);// Reset back to normal size for the rest of the UI

				ImGui::PushID(articleNum + (2 * common->allArticles.size()));
				if (ImGui::Button("Show details") || article.showFullDetailesWindow) {
					article.showFullDetailesWindow = true;
					drawFullDetailesWindow(article,articleNum);
				}
				ImGui::PopID();

				ImGui::SameLine();
				std::string firstWord;
				std::string TOorFROM;
				if (article.isPreferred) {
					firstWord = "remove";
					TOorFROM = " from";
				}
				else {
					firstWord = "add";
					TOorFROM = " to";
				}

				ImGui::PushID(articleNum + (3 * common->allArticles.size()));
				if (ImGui::Button((firstWord + TOorFROM + " prefered").c_str()))
					article.isPreferred = !article.isPreferred;
				ImGui::Text(" ");//go down one line
				ImGui::PopID();
			}
		}
		if (ImGui::Button("Close"))
			showArticlesWindow = false;
	}
	ImGui::End();
}

void initNewsWindow(void* common_ptr) {// main function that initialize the news window
	auto common = (NewsCommonObjects*)common_ptr;
	common->isDataReady.wait(false);// wait until getting notifyed my downloadingnNewsThread that the data have been downloaded
	SetCustomTheme();

	if (showArticlesWindow)
		drawNewsWindow(common);

	else {
		if (showPreferredTitlesWindow)
			drawPreferredTitlesWindow(common);

		int articleNum = 0;
		for (auto& article : common->allArticles) {
			articleNum++;
			if (article.showFullDetailesWindow)
				drawFullDetailesWindow(article,articleNum);
		}
	}
	ResetTheme();
}

void DrawNewsWindowThread::operator()(NewsCommonObjects& common) {// sending the system function to create it with the main gui 
	GuiMain(initNewsWindow, &common);
}