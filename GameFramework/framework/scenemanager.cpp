#include "scenemanager.h"

SceneManager::SceneManager()
	: m_iCurrentSceneIndex(0)
{
}

SceneManager::~SceneManager()
{
	for (std::map<std::string, Scene*>::iterator iter = m_pLoadedScenes.begin(); iter != m_pLoadedScenes.end(); ++iter)
	{
		delete iter->second;
	}
	m_pLoadedScenes.clear();
}

void SceneManager::AddScene(Scene* pScene)
{

}

void SceneManager::RemoveScene(Scene* pScene)
{
}

Scene* SceneManager::GetScene(const char* pcSceneName)
{
	return 0;
}

void SceneManager::SetCurrentScene(int iIndex)
{
	
}

void SceneManager::DebugDraw()
{

}