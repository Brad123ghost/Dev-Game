// COMP710 GP Framework 2022
#ifndef __GAME_H_
#define __GAME_H_

// Library includes:
#include <vector>
#include "imgui_impl_sdl2.h"
#include <string>

// Forward delcarations:
class Renderer;
class Scene;
class InputSystem;
class SoundSystem;
class Sprite;

class Game
{
	// Member methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	bool Initialize();
	bool DoGameLoop();
	void Quit();
	void ToggleDebugWindow();
	bool IsDebugVisible();
protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void SetTargetFPS(int fps);
	void ProcessFrameCounting(float deltaTime);
	void EndIntro();
	void DebugDraw();

private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);

	// Member data:
public:

protected:
	static Game* sm_pInstance;
	Renderer* m_pRenderer;
	InputSystem* m_pInputSystem;
	SoundSystem* m_pSoundSystem;

	std::vector<Scene*> m_scenes;
	int m_iCurrentScene;

	__int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElaspedSeconds;
	int m_iFrameCount;
	int m_iFPS;
	
	__int64 m_iFrameStart;
	__int64 m_iFrameEnd;

	int m_iTargetFPS;
	float m_fTargetFrameTime;
	float m_fLastFrameTime;
	float m_fCurrentFrameTime;
	float m_fAverageFrameTime;

	bool m_ShowDebugWindow;
	bool m_bPauseSimulation;

	Sprite* m_pZapPow[4];

	bool m_bShowSplash;
#ifdef USE_LAG
	float m_mfLag;
	int m_iUpdateCount;
#endif // USE_LAG

	bool m_bLooping;

private:
	bool m_bShowDebugHelp;
	bool m_bShowAssetBrowser;
	bool m_bShowEntityManager;
	bool m_bShowFPS;
	bool m_bShowMode;
	std::string m_sMode;
	bool m_bShowGrid;

	/*float x;
	float y;*/

	/*int x;
	int y;*/
};

#endif // __GAME_H_