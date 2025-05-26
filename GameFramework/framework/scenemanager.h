#pragma once
#ifndef __SCENEMANAGER_H_
#define __SCENEMANAGER_H_

#include <string>
#include <map>

class Scene;

class SceneManager
{
	// Member methods:
public:
	SceneManager();
	~SceneManager();

	void AddScene(Scene* pScene);
	void RemoveScene(Scene* pScene);
	Scene* GetScene(const char* pcSceneName);
	void SetCurrentScene(int iIndex);

	void DebugDraw();
protected:
private:
	SceneManager(const SceneManager& sceneManager);
	SceneManager& operator=(const SceneManager& sceneManager);

	// Member data:
public:
protected:
	std::map<std::string, Scene*> m_pLoadedScenes;
	int m_iCurrentSceneIndex;
private:

};

#endif // __SCENEMANAGER_H_