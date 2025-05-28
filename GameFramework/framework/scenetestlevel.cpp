#include "scenetestlevel.h"

#include "renderer.h"
#include "imgui.h"
#include "sprite.h"
#include "camera.h"
#include "debughelper.h"
#include "texture.h"
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
	std::shared_ptr<NewEntity> player = m_entityManager.CreateEntity(eTag::PLAYER);
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
	sprite->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player->AddComponent<CSprite>(sprite);
	player->AddComponent<CTransform>(Vector2(renderer.GetWidth() /2.f, renderer.GetHeight()/2.f));

	std::shared_ptr<NewEntity> player2 = m_entityManager.CreateEntity(eTag::PLAYER);
	std::shared_ptr<Sprite> sprite2 = std::make_shared<Sprite>();
	sprite2->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player2->AddComponent<CSprite>(sprite2);
	player2->AddComponent<CTransform>(Vector2(250.f, 100.f));

	std::shared_ptr<NewEntity> player3 = m_entityManager.CreateEntity(eTag::PLAYER);
	std::shared_ptr<Sprite> sprite3 = std::make_shared<Sprite>();
	sprite3->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player3->AddComponent<CSprite>(sprite3);
	player3->AddComponent<CTransform>(Vector2(0, 0));
	return true;
}

void SceneTestLevel::Process(float deltaTime, InputSystem& inputSystem)
{
	m_entityManager.Update();
	m_pCamera->Process(deltaTime, inputSystem);
	// If has sprite and transform, update sprite position/rot
	for (auto& e : m_entityManager.GetEntities())
	{
		if (e->GetComponent<CTransform>() && e->GetComponent<CSprite>())
		{
			CTransform* transform = e->GetComponent<CTransform>();
			e->GetComponent<CSprite>()->GetSprite()->SetX(transform->position.x);
			e->GetComponent<CSprite>()->GetSprite()->SetY(transform->position.y);
			e->GetComponent<CSprite>()->GetSprite()->SetAngle(transform->rotation);
		}
	}
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
	for(auto& e : m_entityManager.GetEntities())
	{
		
		if(e->GetComponent<CSprite>())
		{
			e->GetComponent<CSprite>()->GetSprite()->Draw(renderer, currentCam);
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

void SceneTestLevel::EntityManagerDebugDraw(bool& open)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.85f));
	if (ImGui::Begin("Entity Manager", &open))
	{
		
		m_entityManager.DrawDebug();
	}
	ImGui::End();
	ImGui::PopStyleColor();
}