// COMP710 GP Framework 2024

// This include:
#include "game.h"

// Library includes:
#include "iniparser.h"
#include "renderer.h"
#include "logmanager.h"
#include "sceneintro.h"
#include "scenemainmenu.h"
#include "scenepausescreen.h"
#include "scenecheckerboards.h"
#include "scenebouncingballs.h"
#include "sceneballgame.h"
#include "sceneanimatedsprite.h"
#include "scenespaceinvaders.h"
#include "sceneparticle.h"
#include "sceneddll.h"
#include "scenebox2dtest.h"
#include "scenetestlevel.h"
#include "imgui_impl_sdl2.h"
#include "inputsystem.h"
#include "soundsystem.h"
#include "sprite.h"
#include <iostream>
#include "statemanager.h"

// Static Members:
Game* Game::sm_pInstance = 0;

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}
	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game() 
	:m_pRenderer(0)
	, m_bLooping(true)
	, m_pZapPow{0}
	, m_bShowSplash(true)
	, m_bShowDebugHelp(false)
	, m_bShowAssetBrowser(false)
	, m_bShowEntityManager(true)
	, m_bShowFPS(false)
	, m_bShowMode(false)
	//, x(500)
	//, y(10)
{

}

Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;

	delete m_pInputSystem;
	m_pInputSystem = 0;

	for (std::vector<Scene*>::iterator iter = m_scenes.begin(); iter < m_scenes.end(); ++iter)
	{
		delete *iter;
	}
	m_scenes.clear();

	for (int i = 0; i < 4; ++i)
	{
		delete m_pZapPow[i];
	}
	
	delete m_pSoundSystem;
	m_pSoundSystem = 0;

	StateManager::GetInstance().DestroyInstance();
}

void Game::Quit()
{
	m_bLooping = false;
}

Uint64 start, end;

bool Game::Initialize()
{
	start = SDL_GetPerformanceCounter();
	m_sMode = "Release";
#ifdef _DEBUG
	m_sMode = "Debug";
#endif
	m_pSoundSystem = new SoundSystem();
	m_pSoundSystem->CreateSystem();
	m_pSoundSystem->Initialize();


	int bbWidth = 1440;
	int bbHeight = 900;

	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialize(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("[Info] Renderer failed to initialize!");
		return false;
	}

	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();

	m_iLastTime = SDL_GetPerformanceCounter();

	//m_pRenderer->SetClearColor(0, 255, 255);

	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
	m_pInputSystem->ShowMouseCursor(true);
	Scene* pScene;
	// Intro Scene
	pScene = new SceneIntro();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);

	// Main Menu
	pScene = new SceneMainMenu();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);

	// Pause Screen
	pScene = new ScenePauseScreen();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);

	// Checkerboard Scene
	/*pScene = new SceneCheckerboards();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	// Bouncing Ball Scene
	/*pScene = new SceneBouncingBalls();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	// Ball Game Scene
	/*pScene = new SceneBallGame();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	// Animated Sprite Scene
	/*pScene = new SceneAnimatedSprite();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene)*/;

	// Space Invaders Scene
	pScene = new SceneSpaceInvaders();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);

	// Scene Particles
	/*pScene = new SceneParticle();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	// The data driven level load is not fully implemented
	// Scene Data Driven Level Load 
	/*pScene = new SceneDDLL();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	// Scene Box2D Test
	/*pScene = new SceneBox2DTest();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);*/

	pScene = new SceneTestLevel();
	pScene->Initialize(*m_pRenderer, *m_pSoundSystem);
	m_scenes.push_back(pScene);

	// Add Static Text
	// Load static text textures into the Texture Manager...
	m_pRenderer->CreateStaticText("Zap!!", 60);
	m_pRenderer->CreateStaticText("Boom!", 60);
	m_pRenderer->CreateStaticText("Pow!!", 60);
	m_pRenderer->CreateStaticText("Pop!!!", 60);
	
	// Generate sprites that use the static text textures...
	m_pZapPow[0] = m_pRenderer->CreateSprite("Zap!!");
	m_pZapPow[0]->SetX(200);
	m_pZapPow[0]->SetY(200);

	m_pZapPow[1] = m_pRenderer->CreateSprite("Boom!");
	m_pZapPow[1]->SetX(400);
	m_pZapPow[1]->SetY(200);

	m_pZapPow[2] = m_pRenderer->CreateSprite("Pow!!");
	m_pZapPow[2]->SetX(600);
	m_pZapPow[2]->SetY(200);

	m_pZapPow[3] = m_pRenderer->CreateSprite("Pop!!!");
	m_pZapPow[3]->SetX(800);
	m_pZapPow[3]->SetY(200);
	/*m_iCurrentScene = 0;*/
	SetTargetFPS(60);
	//m_pRenderer->GenerateGridData();
	// Timer for initialization
	end = SDL_GetPerformanceCounter();
	float initTime = (end - start) / static_cast<float>(SDL_GetPerformanceFrequency());
	//std::cout << "Initialization Time: " << initTime << " seconds" << std::endl;
	std::string initTimeString = "[Info] Initialization took " + std::to_string(initTime) + " seconds.";
	LogManager::GetInstance().Log(initTimeString.c_str());
	// Set base game state
	StateManager::GetInstance().SetState(GameState::STATE_INTRO);
	return true;
	
}
void Game::EndIntro()
{
	// Skip the intro scene
	m_pRenderer->SetClearColor(0, 255, 255);
	//m_iCurrentScene = 1;
	m_bShowSplash = false;
	StateManager::GetInstance().SetState(GameState::STATE_TEST_LEVEL);
	//StateManager::GetInstance().SetState(GameState::STATE_SPACE_INVADERS);
	//StateManager::GetInstance().SetState(GameState::STATE_MAIN_MENU);
	//m_eGameState = STATE_MAIN_MENU;
}


bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	// TODO: process input here:
	
	m_pInputSystem->ProcessInput();
	
	// Might need to be moved to intro scene or something
	// 
	// Only run the intro scene for the first 4 seconds and showSplash is true
	// Else if the user didn't skip then end it automatically
	if(m_fExecutionTime < 4.0f && m_bShowSplash)
	{
		//m_eGameState = STATE_INTRO;
		m_iCurrentScene = (int)StateManager::GetInstance().GetState();
		if(m_pInputSystem->GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED)
		{
			EndIntro();
		}
	}
	else if (m_bShowSplash){
		EndIntro();
	}

	
	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());

		m_iLastTime = current;

		m_fExecutionTime += deltaTime;
		m_iFrameStart = SDL_GetPerformanceCounter();
		Process(deltaTime);

#ifdef USE_LAG
		m_fLag += deltaTime;
		int innerLag = 0;
		while (m_fLag >= stepSize)
		{
			Process(stepSize);
			m_fLag -= stepSize;
			++m_iUpdateCount;
			++innerLag;
		}
#endif // USE_LAG
		
		//std::cout << m_fTargetFrameTime << std::endl;
		Draw(*m_pRenderer);
		m_iFrameEnd = SDL_GetPerformanceCounter();
		m_fLastFrameTime = (m_iFrameEnd - m_iFrameStart) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.f;


		//m_fCurrentFrameTime = SDL_GetPerformanceCounter();
		//if (m_fTargetFrameTime > 0.0f) {
		//	float targetFrameTime = m_fTargetFrameTime;
		//	m_fLastFrameTime = m_fCurrentFrameTime - m_iLastTime;

		//	// Only sleep if we're significantly under target
		//	// Increased the threshold to prevent double counting
		//	if (m_fLastFrameTime < targetFrameTime) {
		//		float sleepTime = (targetFrameTime - m_fLastFrameTime) * 1000.0f;
		//		SDL_Delay(static_cast<Uint32>(sleepTime));
		//	}
		//}
	}

	return m_bLooping;
}

int laggerSize = 0;
void Game::Process(float deltaTime)
{
	if(m_pInputSystem->GetKeyState(SDL_SCANCODE_F1) == BS_PRESSED)
	{
		m_bShowFPS = !m_bShowFPS;
	}
	ProcessFrameCounting(deltaTime);
	if (StateManager::GetInstance().GetState() != GameState::STATE_RESET)
	{
		m_iCurrentScene = (int)StateManager::GetInstance().GetState();
	}
	else if (StateManager::GetInstance().GetState() == GameState::STATE_RESET)
	{
		m_iCurrentScene = (int)GameState::STATE_SPACE_INVADERS;
	}

	// Checks if the current scene can be paused
	if (StateManager::GetInstance().GetState() == GameState::STATE_SPACE_INVADERS ||
		StateManager::GetInstance().GetState() == GameState::STATE_PAUSE)
	{
		if (m_pInputSystem->GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED)
		{
			//std::cout << "Escape button pressed" << std::endl;
			// if not paused pause else unpause
			if (StateManager::GetInstance().GetState() != GameState::STATE_PAUSE)
			{
				StateManager::GetInstance().SetState(GameState::STATE_PAUSE);
			}
			else
			{
				StateManager::GetInstance().SetState(GameState::STATE_SPACE_INVADERS);
			}
		}
	}

	if (m_bPauseSimulation)
	{
		deltaTime = 0.0f;
	}

	// TODO: Add game objects to process here!
	
	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);
	if (StateManager::GetInstance().GetState() == GameState::STATE_EXIT)
	{
		Quit();
	}

	for (int i = 0; i < laggerSize; i++)
	{
		std::cout << i + i << std::endl;
	}
}
void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;

	renderer.Clear();
	
	// TODO: Add game objects to draw here!
	if (StateManager::GetInstance().GetState() == GameState::STATE_PAUSE)
	{
		m_scenes[(int)GameState::STATE_SPACE_INVADERS]->Draw(renderer);
		m_scenes[(int)GameState::STATE_PAUSE]->Draw(renderer);
	}
	else
	{
		m_scenes[m_iCurrentScene]->Draw(renderer);
	}
	// Uncomment to add text
	/*for (int i = 0; i < 4; ++i)
	{
		m_pZapPow[i]->Draw(renderer);
	}*/
	/*if (!m_bShowSplash)
	{
		DebugDraw();
	}*/
	
	DebugDraw();
	//renderer.DrawAABB(0, 0, 100, 100);
	//renderer.DrawGrid(x, y);
	if (m_bShowGrid)
	{
		//renderer.GenerateGrid(x, y)s;
		//renderer.DrawLine2D({ x, 0 }, { x, y });
		/*renderer.DrawLine2D({ 0,0 }, { 0,0.5 });
		renderer.DrawLine2D({ 0.5,0 }, { 0.5,0.5 });
		renderer.DrawLine2D({ 0,0 }, { 0.5,0 });
		renderer.DrawLine2D({ 0,0.5 }, { 0.5,0.5 });
		renderer.DrawLineFlush();*/
		//renderer.DrawGrid();
	}
	renderer.Present();
	
}

void Game::SetTargetFPS(int fps)
{
	if (fps <= 0)
		fps = 60;
	m_fTargetFrameTime = 1.0f / static_cast<float>(fps);
}

void Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElaspedSeconds += deltaTime;
	
	// Frame Counter:
	if (m_fElaspedSeconds > 1.0f)
	{
		m_fElaspedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}

void Game::DebugDraw()
{
	
	// Main dockspace
	/*ImGui::Begin("Main Dockspace", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground);
	ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
	ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_None);
	ImGui::End();*/


	if (m_bShowMode)
	{
		/*ImGui::SetNextWindowDockID(ImGui::GetMainViewport()->ID, ImGuiCond_Always);*/
		//ImGui::SetNextWindowPos(vp->WorkPos, ImGuiCond_Always);
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::Begin("Release Version", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		ImGui::Text("%s Build", m_sMode.c_str());
		float height = ImGui::GetWindowSize().y;
		ImGui::SetWindowPos(ImVec2(vp->WorkPos.x, vp->WorkPos.y + (vp->Size.y - height)), ImGuiCond_Always);
		ImGui::End();
	}
	// FPS Counter using ImGui 
	if (m_bShowFPS)
	{
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::Begin("FPS Counter", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav); // No title bar, no resize, no move, always auto-resize, no saved settings, no focus on appearing, no nav
		ImGui::Text("%.2f ms  FPS: %d", m_fLastFrameTime, m_iFPS);
		float width = ImGui::GetWindowSize().x;
		ImGui::SetWindowPos(ImVec2(vp->WorkPos.x + ImGui::GetIO().DisplaySize.x - width, vp->WorkPos.y), ImGuiCond_Always);
		ImGui::End();
	}
	if (m_ShowDebugWindow)
	{
		if (m_bShowEntityManager && StateManager::GetInstance().GetState() == GameState::STATE_TEST_LEVEL)
		{
			SceneTestLevel* testLevel = static_cast<SceneTestLevel*>(m_scenes[m_iCurrentScene]);
			testLevel->EntityManagerDebugDraw(m_bShowEntityManager);
		}

		if (m_bShowAssetBrowser)
		{
			ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
			ImGui::Begin("Asset Browser", &m_bShowAssetBrowser, ImGuiWindowFlags_None);
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(ImVec2(0, 0), ImVec2(100, 100), IM_COL32(255, 0, 0, 255), 5.0f, ImDrawFlags_RoundCornersAll);
			m_pRenderer->DrawDebug();
			
			ImGui::End();
		}

		if (m_bShowDebugHelp)
		{
			ImGui::Begin("Help", &m_bShowDebugHelp, ImGuiWindowFlags_None);
			ImGui::SeparatorText("Scene Help");
			ImGui::Text("Here is some text lol");
			ImGui::End();
		}

		// Debug mode watermark
		//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(vp->WorkPos, ImGuiCond_Always);
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::Begin("Watermark", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav); // No title bar, no resize, no move, always auto-resize, no saved settings, no focus on appearing, no nav
		ImGui::Text("Debug Activated");
		ImGui::End();

		ImGui::Begin("Debug Window", &m_ShowDebugWindow, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::MenuItem("Asset Browser", NULL, &m_bShowAssetBrowser);
				/*ImGui::SeparatorText("Frames");
				ImGui::MenuItem("Show FPS", NULL, &showFPS);*/

				ImGui::MenuItem("Scene Entity Manager", NULL, &m_bShowEntityManager);
				
				
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Help", NULL, &m_bShowDebugHelp);
			ImGui::EndMenuBar();
		}
		ImGui::Text("COMP710 GP Framework (%s)", "2024, S2");
		if (ImGui::Button("Quit"))
		{
			Quit();
		}
		ImGui::Text("Active scene:");
		ImGui::BeginDisabled();
		ImGui::SliderInt("##SceneSlider", &m_iCurrentScene, 1, m_scenes.size() - 1, "%d");
		ImGui::EndDisabled();
		ImGui::Separator();
		if (ImGui::BeginTabBar("Main##test"))
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				if (ImGui::Button(m_bPauseSimulation ? "Unpause simulation" : "Pause simulation"))
				{
					m_bPauseSimulation = !m_bPauseSimulation;
				}
				m_scenes[m_iCurrentScene]->SceneInfoDraw();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Log"))
			{
				// Log
				LogManager::GetInstance().DebugDraw();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginItemTooltip())
			{
				ImGui::Text("Shows Logging");
				ImGui::Text("Scroll to the bottom to resume auto-scroll.");
				ImGui::EndTooltip();
			}
			//if(ImGui::BeginTabItem("Textures"))
			//{
			//	// Textures
			//	m_pRenderer->DrawDebug();
			//	ImGui::EndTabItem();
			//}
			if (ImGui::BeginTabItem("Debug##test"))
			{
				m_scenes[m_iCurrentScene]->DebugDraw();
				//ImGui::SeparatorText("Grid");
				//ImGui::Checkbox("Show Grid", &m_bShowGrid);
				//if(m_bShowGrid)
				//{
				//	ImGui::SliderInt("Grid Size", &x, 1, 500, "%d");
				//	ImGui::SliderInt("Grid Size", &y, 1, 100, "%d");
				//	/*ImGui::SliderFloat ("X", &x, -2, 2, "%f");
				//	ImGui::SliderFloat("Y", &y, -2, 2, "%f");*/
				//}
				ImGui::SeparatorText("Version");
				ImGui::Checkbox("Show Version", &m_bShowMode);
				ImGui::SeparatorText("FPS");
				ImGui::Checkbox("Show FPS", &m_bShowFPS);
				ImGui::SliderInt("Lag Slider", &laggerSize, 0, 100, "%d");
				if (ImGui::BeginItemTooltip())
				{
					ImGui::Text("Introduces lag via a for loop that runs every frame.");
					ImGui::EndTooltip();
				}
				{
					ImGui::BeginDisabled();
					const char* fpsLimitItems[] = { "60", "120", "144", "240", "360" };
					static int item_selected_idx = 0;
					const char* combo_preview_value = fpsLimitItems[item_selected_idx];
					if (ImGui::BeginCombo("FPS Limit", combo_preview_value))
					{
						for (int n = 0; n < IM_ARRAYSIZE(fpsLimitItems); n++)
						{
							bool isSelected = (m_iTargetFPS == atoi(fpsLimitItems[n]));
							if (ImGui::Selectable(fpsLimitItems[n], isSelected))
							{
								item_selected_idx = n;
								m_iTargetFPS = atoi(fpsLimitItems[n]);
								SetTargetFPS(m_iTargetFPS);
							}
							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::EndDisabled();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		


		ImGui::End();
	}
}


void Game::ToggleDebugWindow()
{
	m_ShowDebugWindow = !m_ShowDebugWindow;
	//std::cout << "Debug Pressed" << std::endl;
	std::cout << m_ShowDebugWindow << std::endl;
	//m_pInputSystem->ShowMouseCursor(m_ShowDebugWindow);
}

bool Game::IsDebugVisible()
{
	return m_ShowDebugWindow;
}