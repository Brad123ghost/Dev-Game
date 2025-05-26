// COMP710 GP Framework 2024

// This include:
#include "logmanager.h"

// Library includes:
#include <Windows.h>
#include "imgui.h"
#include <iostream>

// Static Members:
LogManager* LogManager::sm_pInstance = 0;

LogManager& LogManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new LogManager();
	}
	return (*sm_pInstance);
}

void LogManager::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void LogManager::Log(const char* pcMessage)
{
	OutputDebugStringA(pcMessage);
	OutputDebugStringA("\n");
	m_logHistory.push_back(pcMessage);
	std::cout << pcMessage << std::endl;
}

void LogManager::DebugDraw()
{
	/*ImGui::Checkbox("Show Logging", &m_bShowLogMessages);
	if (m_bShowLogMessages)
	{*/
		//ImGui::Text("Log Manager:");
		ImGui::BeginChild("Log", ImVec2(ImGui::GetContentRegionAvail().x, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
		const float currentScrollY = ImGui::GetScrollY();
		const float maxScrollY = ImGui::GetScrollMaxY();
		//std::reverse(m_logHistory.begin(), m_logHistory.end());

		std::vector<std::string>::iterator iter = m_logHistory.begin();

		while (iter != m_logHistory.end())
		{
			ImGui::Text((*iter).c_str());
			++iter;
		}

		if (currentScrollY >= maxScrollY - 20.0f) {
			ImGui::SetScrollHereY(1.0f);
		}
		//std::reverse(m_logHistory.begin(), m_logHistory.end());
		ImGui::EndChild();
	//}
}