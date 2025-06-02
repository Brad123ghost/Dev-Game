#include "scenespaceinvaders.h"

#include "renderer.h"
#include "lasercanon.h"
#include "laserbullet.h"
#include "alieninvader.h"
#include "imgui.h"
#include "logmanager.h"
#include "inputsystem.h"
#include "soundsystem.h"
#include "sprite.h"
#include <iostream>
#include "iniparser.h"
#include "statemanager.h"
#include "inlinehelper.h"

SceneSpaceInvaders::SceneSpaceInvaders()
	: m_pRenderer(0)
	, m_pLaserCanon(0)
	, m_pLaserBullet(0)
	, m_pAlienInvaders(0)
	, m_fPlayerMovementSpeed(0)
	, m_fPlayerBulletRechargeTimer(0)
	, m_fPlayerLastBulletTime(0)
	, m_invaderMoveTimer(0)
	, m_ALIEN_PER_ROW(0)
	, m_ALIEN_PER_COLUMN(0)
	, m_invaderBlockStart(0)
	, m_invaderBlockEnd(0)
	, m_defaultInvaderBlockStart(0)
	, m_defaultInvaderBlockEnd(0)
	, m_MoveLeft(true)
	, m_MoveDown(false)
	, m_pBackground(0)
	, m_pText(0)
	, m_pIniParser(0)
	, m_bDrawAABB(false)
	, m_iLowerBound(0)
	, m_iUpperBound(0)
	, m_bEnableCamera(false)
	, m_pPlayerCamera(nullptr)
	, m_bShowGrid(false)
{

}

SceneSpaceInvaders::~SceneSpaceInvaders()
{
	delete m_pLaserCanon;
	delete m_pBackground;
	delete m_pText;

	for (std::vector<Entity*>::iterator iter = m_pLaserBullet.begin(); iter < m_pLaserBullet.end(); ++iter)
	{
		delete* iter;
	}
	m_pLaserBullet.clear();

	for (std::vector<Entity*>::iterator iter = m_pAlienInvaders.begin(); iter < m_pAlienInvaders.end(); ++iter)
	{
		delete* iter;
	}
	m_pAlienInvaders.clear();

	delete m_pIniParser;
	m_pIniParser = 0;
	delete tempback;
	delete tempback2;
	delete tempback3;

	delete m_pPlayerCamera;
}

struct test
{
	int row;
	int column;
};

std::map<std::string, test> testMap;

bool SceneSpaceInvaders::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	// Data is being loaded from test.ini
	m_pIniParser = new IniParser();
 	m_pIniParser->LoadIniFile("test.ini");

	// Load player settings form ini file
	m_fPlayerMovementSpeed = m_pIniParser->GetValueAsFloat("Player", "movementSpeed");
	m_fPlayerBulletRechargeTimer = m_pIniParser->GetValueAsFloat("Player", "bulletRechargeTimer");
	
	// Set up sound system
	m_pSoundSystem = &soundSystem;
	m_pSoundSystem->CreateSound("shoot", "audio\\SpaceInvaders\\laser-shoot.wav");
	m_pSoundSystem->CreateSound("hit", "audio\\SpaceInvaders\\alien-hit.wav");
	m_pSoundSystem->CreateSound("explode", "audio\\SpaceInvaders\\explosion.wav");

	/*renderer.CreateStaticText("Test", 48);
	m_pText = renderer.CreateSprite("Test");
	m_pText->SetX(renderer.GetWidth() / 2.0f);
	m_pText->SetY(renderer.GetHeight() / 2.0f);*/

	// Set the enemies per row and column
	const int numOfLevels = 3;
	for (int i = 0; i < numOfLevels; i++)
	{
		std::string temp = "level" + std::to_string(i+1);
		int row = m_pIniParser->GetValueAsInt("WaveSizes", temp+"EnemiesPerRow");
		int column = m_pIniParser->GetValueAsInt("WaveSizes", temp+"EnemiesPerColumn");

		testMap.insert(std::pair<std::string, test>(temp, { row, column }));
	}


	//tempback = renderer.CreateSprite("sprites\\crate.png");
	tempback = renderer.CreateSprite("sprites\\yes.jpg");
	tempback2 = renderer.CreateSprite("sprites\\test.jpg");
	tempback2->SetY(tempback->GetHeight());
	tempback3 = renderer.CreateSprite("sprites\\yep.jpg");
	tempback3->SetX(tempback2->GetWidth());
	tempback3->SetY(tempback->GetHeight());

	m_iMaxWidth = renderer.GetWidth();
	m_iMaxHeight = renderer.GetHeight();
	m_pBackground = renderer.CreateSprite("sprites\\Space_Invader_BG.png");
	m_pBackground->SetX(m_iMaxWidth /2.0f);
	m_pBackground->SetY(m_iMaxHeight / 2.0f);

	// Set the bounds based on the background image
	// Minus 45 to account for some offset set somewhere idk lmao
	m_iLowerBound = m_pBackground->GetX() - m_pBackground->GetWidth() / 2.0f + 45;
	m_iUpperBound = m_pBackground->GetX() + m_pBackground->GetWidth() / 2.0f - 45;

	m_pRenderer = &renderer;
	LaserCanon* player = new LaserCanon();
	player->Initialize(renderer);
	player->SetSpeed(m_fPlayerMovementSpeed);
	m_pLaserCanon = player;

	m_pPlayerCamera = new Camera(renderer.GetWidth(), renderer.GetHeight());
	// [THIS IS AN OLD COMMENT] - This was the case when I had set values thou it still s somewhat valid :D
	// Since I have a default offset of 30 between each alien, and there is 11 columns
	// 5*30 = 150 for offset and the block of aliens to be centered


	// Always set level 1 as default
	LoadLevel(Level::LEVEL_1);
	SpawnInvaders();

	return true;
}
int camSpeed = 30;
Vector2 moveVector;
void SceneSpaceInvaders::Process(float deltaTime, InputSystem& inputSystem)
{
	
	if(StateManager::GetInstance().GetState() == GameState::STATE_RESET)
	{
		ResetGame();
	}

	m_pSoundSystem->Update();
	/*m_pBackground->Process(deltaTime);*/
	ButtonState spaceState = (inputSystem.GetKeyState(SDL_SCANCODE_SPACE));
	m_invaderMoveTimer += deltaTime;

	// Handle Pause
	/*if(inputSystem.GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED)
	{
		StateManager::GetInstance().SetState(GameState::STATE_PAUSE);
	}*/

	// Shooting should be handled by the laser canon class and not the game itself
	// cause technically the canon is the wepaon.
	if (m_fPlayerLastBulletTime * 1000 > 0)
	{
		m_fPlayerLastBulletTime -= deltaTime * 1000;
	}
	
	if (spaceState == BS_PRESSED || spaceState == BS_HELD)
	{
		if (m_fPlayerLastBulletTime <= 0)
		{
			PlaySound("shoot");
			SpawnBullet();
			m_fPlayerLastBulletTime = m_fPlayerBulletRechargeTimer * 1000;
		}
	}
	m_pLaserCanon->Process(deltaTime, inputSystem);

	// Movement/new bound logic for invaders
	{
		if (m_pAlienInvaders.size() > 0)
		{
			if (m_invaderMoveTimer > 0.25f)
			{
				float currentLowest = m_defaultInvaderBlockEnd;
				float currentHighest = m_defaultInvaderBlockStart;
				/*float currentLowest = CamScreenToWorld({ static_cast<float>(m_defaultInvaderBlockStart), 0 }, *m_pPlayerCamera).x;
				float currentHighest = CamScreenToWorld({ static_cast<float>(m_defaultInvaderBlockEnd), 0 }, *m_pPlayerCamera).x;*/
				// Recalculating current bounds 
				for(std::vector<Entity*>::iterator iter = m_pAlienInvaders.begin(); iter < m_pAlienInvaders.end(); ++iter)
				{
					//Vector2 worldPos = m_pPlayerCamera->ScreenToWorld((*iter)->GetPosition() - m_pPlayerCamera->GetPosition() + m_pPlayerCamera->GetScreenSize() / 2.0f); // Get the world position of the invader
					//Vector2 worldPos = CamScreenToWorld((*iter)->GetPosition(), *m_pPlayerCamera);
					/*if (worldPos.x < currentLowest)
					{
						currentLowest = worldPos.x;
					}
					if (worldPos.x > currentHighest)
					{
						currentHighest = worldPos.x;
					}*/
					if ((*iter)->GetPosition().x < currentLowest)
					{
						currentLowest = (*iter)->GetPosition().x;
					}
					if ((*iter)->GetPosition().x > currentHighest)
					{
						currentHighest = (*iter)->GetPosition().x;
					}
				}

				for (std::vector<Entity*>::iterator iter = m_pAlienInvaders.begin(); iter < m_pAlienInvaders.end(); ++iter)
				{
					AlienInvader* temp = static_cast<AlienInvader*>(*iter);
					if(m_MoveDown)
					{
						temp->MoveDown();
					}
					else if (!m_MoveLeft)
					{
						temp->MoveRight();
					}
					else if (m_MoveLeft)
					{
						temp->MoveLeft();
					}
					(*iter)->Process(deltaTime, inputSystem);
				}

				m_invaderBlockStart = currentLowest;
				m_invaderBlockEnd = currentHighest;

				/*if (m_MoveLeft && m_invaderBlockStart <= CamScreenToWorld({ static_cast<float>(m_iLowerBound), 0 }, *m_pPlayerCamera).x)
				{
					m_MoveLeft = false;
					m_MoveDown = true;
				}
				else if (!m_MoveLeft && m_invaderBlockEnd >= CamScreenToWorld({ static_cast<float>( m_iUpperBound), 0 }, *m_pPlayerCamera).x)
				{
					m_MoveLeft = true;
					m_MoveDown = true;
				}
				if (m_MoveDown)
				{
					m_MoveDown = false;
				}*/
				if (m_MoveLeft && m_invaderBlockStart <= m_iLowerBound)
				{
					m_MoveLeft = false;
					m_MoveDown = true;
				}
				else if (!m_MoveLeft && m_invaderBlockEnd >= m_iUpperBound)
				{
					m_MoveLeft = true;
					m_MoveDown = true;
				}
				if (m_MoveDown)
				{
					m_MoveDown = false;
				}
				m_invaderMoveTimer = 0;
			}
		}
	}

	if (m_pLaserBullet.size() > 0)
	{
		for (std::vector<Entity*>::iterator iter = m_pLaserBullet.begin(); iter < m_pLaserBullet.end();)
		{
			if ((*iter)->IsAlive() == false)
			{
				delete (*iter);
				iter = m_pLaserBullet.erase(iter);
			}
			else
			{
				(*iter)->Process(deltaTime, inputSystem);
				++iter;
			}
		}
	}
	// Double for loop for projects and invaders to detect collision
	{
		if (m_pLaserBullet.size() > 0 && m_pAlienInvaders.size() > 0)
		{
			for (std::vector<Entity*>::iterator bulletIter = m_pLaserBullet.begin(); bulletIter < m_pLaserBullet.end();)
			{
				for (std::vector<Entity*>::iterator alienIter = m_pAlienInvaders.begin(); alienIter < m_pAlienInvaders.end();)
				{
					if ((*bulletIter)->IsCollidingWith(*(*alienIter)))
					{
						(*bulletIter)->SetDead();
						(*alienIter)->SetDead();
						PlaySound("hit");
						PlaySound("explode");
					}
					if (!(*alienIter)->IsAlive())
					{
						delete(*alienIter);
						alienIter = m_pAlienInvaders.erase(alienIter);
					}
					else
					{
						++alienIter;
					}
				}
				if (!(*bulletIter)->IsAlive())
				{
					delete(*bulletIter);
					bulletIter = m_pLaserBullet.erase(bulletIter);
				}
				else
				{
					++bulletIter;
				}
			}
		}
	}

	if (m_bDrawAABB)
	{

		m_pLaserCanon->DrawAABB(true);
		for (Entity* bullet : m_pLaserBullet)
		{
			bullet->DrawAABB(true);
		}
		for (Entity* alien : m_pAlienInvaders)
		{
			alien->DrawAABB(true);
		}
	}
	else
	{
		m_pLaserCanon->DrawAABB(false);
		for (Entity* bullet : m_pLaserBullet)
		{
			bullet->DrawAABB(false);
		}
		for (Entity* alien : m_pAlienInvaders)
		{
			alien->DrawAABB(false);
		}
	}

	
	Vector2 mousePos = inputSystem.GetMousePosition();
	Vector2 worldTarget = m_pPlayerCamera->ScreenToWorld(mousePos);
	
	/*std::cout << "Mouse Position: " << mousePos.x << ", " << mousePos.y << "	World: " << worldTarget.x << ", " << worldTarget.y << std::endl;
	std::cout << "Camera Position: " << m_pRenderer->GetCamera()->GetPosition().x << ", " << m_pRenderer->GetCamera()->GetPosition().y << std::endl;*/
	if (m_bEnableCamera)
	{
		if (m_pPlayerCamera->GetCameraType() == CameraType::CAMERA_FOLLOW)
		{
			Vector2 playerWorldPos = m_pLaserCanon->GetPosition();
			m_pPlayerCamera->SetPosition(playerWorldPos.x - m_pRenderer->GetWidth()/2.0f, 0);
		}
		else if (m_pPlayerCamera->GetCameraType() == CameraType::CAMERA_FREE_CAM)
		{
			m_pPlayerCamera->ProcessInput(deltaTime, inputSystem);
		}

		// Mouse wheel zoom
		int mouseWheel = inputSystem.GetMouseScrollWheel().y;
		if (mouseWheel != 0) {
			/*float zoomAmount = 1.0f + (mouseWheel > 0 ? 0.1f : -0.1f);*/
			Vector2 mousePos = inputSystem.GetMousePosition();
			Vector2 worldTarget = m_pPlayerCamera->ScreenToWorld(mousePos);
			//std::cout << "Mouse Position: " << mousePos.x << ", " << mousePos.y << "	World: " << worldTarget.x << ", " << worldTarget.y << std::endl;
			bool zoomIn = mouseWheel > 0 ? true : false;
			m_pPlayerCamera->ZoomTowardsPoint(worldTarget, zoomIn);
			/*m_pRenderer->GetCamera()->Zoom(zoomAmount);*/
		}

	}
}

void SceneSpaceInvaders::Draw(Renderer& renderer)
{
	//renderer.DrawGrid();
	/*if (m_bShowGrid)
	{*/

	//}
	tempback->Draw(renderer, m_pPlayerCamera);
	tempback2->Draw(renderer, m_pPlayerCamera);
	tempback3->Draw(renderer, m_pPlayerCamera);
	m_pBackground->Draw(renderer, m_pPlayerCamera);
	m_pLaserCanon->Draw(renderer, m_pPlayerCamera);
	if (m_pLaserBullet.size() > 0)
	{
		for (std::vector<Entity*>::iterator iter = m_pLaserBullet.begin(); iter < m_pLaserBullet.end(); ++iter)
		{
			(*iter)->Draw(renderer, m_pPlayerCamera);
		}
	}
	if (m_pAlienInvaders.size() > 0)
	{
		for (std::vector<Entity*>::iterator iter = m_pAlienInvaders.begin(); iter < m_pAlienInvaders.end(); ++iter)
		{
			(*iter)->Draw(renderer, m_pPlayerCamera);
		}
	}
	//m_pText->Draw(renderer);
	//renderer.DrawLine2D({ 0,0 }, { 0,0.5 });
	/*renderer.DrawLine2D({1,0}, {1,0.5});
	renderer.DrawLine2D({1,1}, {1,1});*/
	/*renderer.DrawLine2D({ 0.5,0 }, { 0.5,0.5 });
	renderer.DrawLine2D({ 0,0 }, { 0.5,0 });
	renderer.DrawLine2D({ 0,0.5 }, { 0.5,0.5 });*/
	//renderer.DrawLineFlush(m_pPlayerCamera);
}

void SceneSpaceInvaders::SceneInfoDraw()
{

}

void SceneSpaceInvaders::DebugDraw()
{
	bool debug = false;
	ImGui::Text("Scene: Space Invaders");
	ImGui::Separator();
	if (ImGui::Button("Reset Game"))
	{
		StateManager::GetInstance().SetState(GameState::STATE_RESET);
	}
	if (ImGui::CollapsingHeader("Details"))
	{
			ImGui::Text("Game Details");
			ImGui::Text("Laser Bullets: %d", m_pLaserBullet.size());
			ImGui::Text("Number of Aliens: %d", m_pAlienInvaders.size());
			ImGui::Text("Block Start: %d", m_invaderBlockStart);
			ImGui::Text("Block End: %d", m_invaderBlockEnd);
			ImGui::Text("Block Start Default: %d", m_defaultInvaderBlockStart);
			ImGui::Text("Block End Default: %d", m_defaultInvaderBlockEnd);
	}
	if(ImGui::CollapsingHeader("Camera Settings"))
	{
		ImGui::Checkbox("Enable Camera", &m_bEnableCamera);
		if (m_bEnableCamera)
		{

			Camera* activeCam = m_pPlayerCamera;
		
			const char* camTypeItems[] = { "Free Cam", "Follow"};
			static int item_selected_idx = 0;
			const char* combo_preview_value = camTypeItems[item_selected_idx];
			ImGui::Text("Camera Type");
			if (ImGui::BeginCombo("##Camera_Type", combo_preview_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(camTypeItems); n++)
				{
					//bool isSelected = (m_iTargetFPS == camTypeItems[n]));
					if (ImGui::Selectable(camTypeItems[n], false))
					{
						item_selected_idx = n;
						
						activeCam->SetCameraType((CameraType)n);
					}
					/*if (isSelected)
						ImGui::SetItemDefaultFocus();*/
				}
				ImGui::EndCombo();
			}
			if (item_selected_idx != 1)
			{
				if(ImGui::SliderInt("Move Speed", &camSpeed, 0, 60))
					activeCam->SetCamSpeed(camSpeed);

				ImGui::Text("Current Cam Speed: %d", m_pPlayerCamera->GetCamSpeed());
			}
			if (moveVector.x != 0 && moveVector.y != 0) {
				moveVector.Normalize();
			}
			ImGui::Text("Vector (%.3f, %.3f)", moveVector.x, moveVector.y);
			ImGui::Text("Camera Position");
			Vector2 cameraPos = activeCam->GetPosition();
			ImGui::Text("X: %f", cameraPos.x);
			ImGui::Text("Y: %f", cameraPos.y);
			ImGui::Text("Zoom: %f", activeCam->GetZoom());
			if (ImGui::Button("Reset Camera"))
			{
				activeCam->ResetCamera();
			}
			
			/*struct dump
			{
				int x;
				int y;
			};

			dump tempDump = { activeCam->GetPosition().x, activeCam->GetPosition().y};

			if (ImGui::SliderInt("Camera X", &tempDump.x, -m_iMaxWidth, m_iMaxWidth))
			{
				activeCam->SetPosition(tempDump.x, 0);
			}
			if (ImGui::SliderInt("Camera Y", &tempDump.y, -m_iMaxHeight, m_iMaxHeight))
			{
				activeCam->SetPosition(0, tempDump.y);
			}*/
		}
	}
	if (ImGui::CollapsingHeader("Player Settings"))
	{
		{
			ImGui::Text("Player Movement Speed");
			if (ImGui::SliderFloat("##MoveSpeedSlider", &m_fPlayerMovementSpeed, 0.0f, 20.0f))
			{
				m_pLaserCanon->SetSpeed(m_fPlayerMovementSpeed);
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				m_fPlayerMovementSpeed = m_pIniParser->GetValueAsFloat("Player", "movementSpeed");
				m_pLaserCanon->SetSpeed(m_fPlayerMovementSpeed);
			}
		}
		{
			ImGui::SliderFloat("Recharge Timer", &m_fPlayerBulletRechargeTimer, 0.2f, 2.0f);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				m_fPlayerBulletRechargeTimer = m_pIniParser->GetValueAsFloat("Player", "bulletRechargeTimer");
		}
	}
	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Checkbox("Draw AABB", &m_bDrawAABB);
		ImGui::Checkbox("Draw Grid", &m_bShowGrid);
	}

	/*ImGui::SliderFloat("Width", &tempWidth, 0, 2);
	ImGui::SliderFloat("Height", &tempHeight, 0, 2);*/
}

void SceneSpaceInvaders::SpawnBullet()
{
	LaserBullet* newLaser = new LaserBullet();
 	newLaser->Initialize(*m_pRenderer, {m_pLaserCanon->GetPosition(), 0});
	m_pLaserBullet.push_back(newLaser);
}

void SceneSpaceInvaders::SpawnInvaders()
{
	const float alienOffSet = 150.0f;
	float alienStart = m_iMaxWidth / 2.0f - alienOffSet;
	m_invaderBlockStart = alienStart;
	m_invaderBlockEnd = alienStart + m_ALIEN_PER_ROW * 30;
	m_defaultInvaderBlockStart = m_iLowerBound;
	m_defaultInvaderBlockEnd = m_iUpperBound;
	for (int i = 0; i < m_ALIEN_PER_COLUMN; ++i)
	{
		for (int k = 0; k < m_ALIEN_PER_ROW; ++k)
		{
			AlienInvader* newInvader = new AlienInvader();
			newInvader->Initialize(*m_pRenderer);
			newInvader->SetSpawnPosition({ alienStart + 30.0f * k ,300 + 30.0f * i });
			m_pAlienInvaders.push_back(newInvader);
		}
	}
	std::string logMsg = "[Space Invaders] Level loaded " + std::to_string(static_cast<int>(m_iCurrentLevel));
	LogManager::GetInstance().Log(logMsg.c_str());
}

void SceneSpaceInvaders::StartGame()
{

}

void SceneSpaceInvaders::LoadLevel(Level level)
{
	std::string logMsg = "[Space Invaders] Loading level " + std::to_string(static_cast<int>(level));
	LogManager::GetInstance().Log(logMsg.c_str());
	m_iCurrentLevel = level;
	m_ALIEN_PER_ROW = testMap["level" + std::to_string(static_cast<int>(level))].row;
	m_ALIEN_PER_COLUMN = testMap["level" + std::to_string(static_cast<int>(level))].column;
}

void SceneSpaceInvaders::ResetGame()
{
	LoadLevel(m_iCurrentLevel);
	StateManager::GetInstance().SetState(GameState::STATE_SPACE_INVADERS);
	for (std::vector<Entity*>::iterator iter = m_pLaserBullet.begin(); iter < m_pLaserBullet.end(); ++iter)
	{
		delete* iter;
	}
	m_pLaserBullet.clear();

	for (std::vector<Entity*>::iterator iter = m_pAlienInvaders.begin(); iter < m_pAlienInvaders.end(); ++iter)
	{
		delete* iter;
	}
	m_pAlienInvaders.clear();
	SpawnInvaders();
	LogManager::GetInstance().Log("[Space Invaders] Game has been reset");
}

void SceneSpaceInvaders::PlaySound(const char* sound)
{
	m_pSoundSystem->PlaySound(sound);
}