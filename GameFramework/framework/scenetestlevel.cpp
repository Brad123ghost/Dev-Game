#include "scenetestlevel.h"

#include "renderer.h"
#include "imgui.h"
#include "sprite.h"
#include "camera.h"
#include "debughelper.h"

SceneTestLevel::SceneTestLevel()
	: m_bShowGrid(false)
	, m_bDrawAABB(false)
	, m_pTestSprite(0)
	, m_pCamera(0)
	, gridSize(10)
	, cellSize(32)

{
}

SceneTestLevel::~SceneTestLevel()
{
	delete m_pTestSprite;
	m_pTestSprite = 0;

	delete m_pCamera;
	m_pCamera = 0;
}

bool SceneTestLevel::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pCamera = new Camera(renderer.GetWidth(), renderer.GetHeight());

	renderer.SetClearColor(0, 0, 0);
	m_pTestSprite = renderer.CreateSprite("sprites\\crate.png");
	m_pTestSprite->SetX(100);
	m_pTestSprite->SetY(100);
	
	// Add player
	std::shared_ptr<NewEntity> player = m_pEntityManager.CreateEntity(eTag::PLAYER);
	m_pEntityManager.Update();
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
	sprite->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	//player->cSprite = std::make_shared<CSprite>(sprite);
	//player->Add<CSprite>(sprite);
	player->AddComponent<CSprite>(sprite);
	return true;
}

void SceneTestLevel::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pCamera->Process(deltaTime, inputSystem);
	// Process the entities
	//m_pEntityManager.Update();
}

void SceneTestLevel::Draw(Renderer& renderer)
{
	//renderer.DrawLine2D({ 0,0 }, { 0,1 });
	/*renderer.DrawLine2D({ 400,700 }, { 400,750 });
	renderer.DrawLine2D({ 400,700 }, { 450,700 });
	renderer.DrawLine2D({ 450,700 }, { 450,750 });
	renderer.DrawLine2D({ 400,750 }, { 450,750 });*/
	//renderer.DrawGrid();
	Camera* currentCam = m_pCamera->IsEnabled() ? m_pCamera : nullptr;
	if (m_bShowGrid)
	{
		renderer.GenerateGrid(gridSize,cellSize);
		renderer.DrawLineFlush(currentCam);

	}
	m_pTestSprite->Draw(renderer, currentCam);

	// Draw Entities
	for(auto& entity : m_pEntityManager.GetEntities())
	{
		auto spriteComponent = entity->GetComponent<CSprite>();
		if(spriteComponent)
		{
			spriteComponent->GetSprite()->Draw(renderer, currentCam);
		}
	}

	if(m_bDrawAABB)
	{
		renderer.DrawAABB(0, 0, 2, 2);
	}
}

void SceneTestLevel::SceneInfoDraw()
{
	ImGui::Text("Scene: Test Level");
	DebugHelper::DrawCameraDebug(m_pCamera);
}

void SceneTestLevel::DebugDraw()
{
	DebugHelper::DrawGridDebug(gridSize, cellSize, m_bShowGrid);

	//ImGui::Checkbox("Show Grid", &m_bShowGrid);
}