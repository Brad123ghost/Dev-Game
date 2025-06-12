#include "scenetestlevel.h"

#include "renderer.h"
#include "imgui.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "camera.h"
#include "debughelper.h"
#include "texture.h"
#include "logmanager.h"
#include "sprocessinput.h"

SceneTestLevel::SceneTestLevel()
	: m_bShowGrid(false)
	, m_bDrawAABB(false)
	, m_pTestSprite(0)
	, m_pCamera(0)
	, gridSize(10)
	, cellSize(32)
	, m_pRenderer(0)
{
}

SceneTestLevel::~SceneTestLevel()
{

	delete m_pTestSprite;
	m_pTestSprite = 0;

	delete m_pCamera;
	m_pCamera = 0;

	delete m_pWalkLeft;
	m_pWalkLeft = 0;

	delete m_pWalkRight;
	m_pWalkRight = 0;

}

bool SceneTestLevel::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	m_pWalkLeft = renderer.CreateAnimatedSprite("sprites\\walking_left.png");
	m_pWalkLeft->SetX(100);
	m_pWalkLeft->SetY(100);
	m_pWalkLeft->SetupFrames(64, 64);
	m_pWalkLeft->SetFrameDuration(0.1f);
	m_pWalkLeft->SetLooping(true);
	m_pWalkLeft->Animate();

	m_pWalkRight = renderer.CreateAnimatedSprite("sprites\\walking_right.png");
	m_pWalkRight->SetX(100);
	m_pWalkRight->SetY(200);
	m_pWalkRight->SetupFrames(64, 64);
	m_pWalkRight->SetFrameDuration(0.1f);
	m_pWalkRight->SetLooping(true);
	m_pWalkRight->Animate();

	m_pRenderer = &renderer;
	m_pCamera = new Camera(renderer.GetWidth(), renderer.GetHeight());

	renderer.SetClearColor(0, 0, 0);
	m_pTestSprite = renderer.CreateSprite("sprites\\crate.png");
	m_pTestSprite->SetX(300);
	m_pTestSprite->SetY(300);
	m_pTestSprite->SetScale(0.5f);

	// Add player
	std::shared_ptr<NewEntity> player = m_entityManager.CreateEntity("Player", eTag::PLAYER);
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
	sprite->Initialize(*renderer.CreateTexture("sprites\\ball.png"));
	player->AddComponent<CSprite>(sprite);
	player->AddComponent<CTransform>(Vector2(0, 0));
	player->AddComponent<CInput>();

	std::shared_ptr<NewEntity> player2 = m_entityManager.CreateEntity("Dummy 1", eTag::DEFAULT);
	std::shared_ptr<Sprite> sprite2 = std::make_shared<Sprite>();
	sprite2->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player2->AddComponent<CSprite>(sprite2);
	player2->AddComponent<CTransform>(Vector2(-250.f, -250));

	std::shared_ptr<NewEntity> player3 = m_entityManager.CreateEntity("Dummy 1", eTag::DEFAULT);
	std::shared_ptr<Sprite> sprite3 = std::make_shared<Sprite>();
	sprite3->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player3->AddComponent<CSprite>(sprite3);
	player3->AddComponent<CTransform>(Vector2(-300, 300));
	return true;
}

void SceneTestLevel::replaceTexture(std::shared_ptr<Sprite> sprite, const char* texturePath)
{
	Texture* newTexture = m_pRenderer->CreateTexture(texturePath);
	if (newTexture)
	{
		//newTexture->SetActive();
		sprite->ReplaceTexture(*newTexture);
	}
	else
	{
		LogManager::GetInstance().Log("Failed to replace texture: Texture is null.");
	}

}
static int counter = 0;
static float timer = 0;
void SceneTestLevel::Process(float deltaTime, InputSystem& inputSystem)
{
	// Process animation temp 
	if (m_pWalkLeft->IsAnimating())
	{
		m_pWalkLeft->Process(deltaTime);
	}
	if (m_pWalkRight->IsAnimating())
	{
		m_pWalkRight->Process(deltaTime);
	}
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
			e->GetComponent<CSprite>()->GetSprite()->SetXScale(transform->scale.x);
			e->GetComponent<CSprite>()->GetSprite()->SetYScale(transform->scale.y);
		}
	}

    
	// If entity has input and is player
	auto& t = m_entityManager.GetEntities(eTag::PLAYER);
	SProcessInput::ProcessPlayerInput(deltaTime, m_entityManager, inputSystem, *m_pCamera);
	/*if (m_entityManager.GetEntities(eTag::PLAYER) != NULL)
	{
		CInput* input = player->GetComponent<CInput>();
		if (input)
		{
			CTransform* transform = player->GetComponent<CTransform>();
			if (input->m_bUp) transform->position.y -= 100.f * deltaTime;
			if (input->m_bDown) transform->position.y += 100.f * deltaTime;
			if (input->m_bLeft) transform->position.x -= 100.f * deltaTime;
			if (input->m_bRight) transform->position.x += 100.f * deltaTime;
		}
	}*/
	
	// Process the entities
	////m_pEntityManager.Update();
	if (timer < 1.f)
	{
		timer += deltaTime;
	}
	else
	{
		timer = 0.f;
		counter++;
	}
}

void SceneTestLevel::Draw(Renderer& renderer)
{
	// Animation draw
	if (m_pWalkLeft->IsAnimating())
	{
		m_pWalkLeft->Draw(renderer);
	}
	if (m_pWalkRight->IsAnimating())
	{
		m_pWalkRight->Draw(renderer);
	}

	/*std::string title = "Test Level - " + std::to_string(counter);
	renderer.DrawText(title.c_str(), 10, 10, 1.0f);*/
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
	m_pTestSprite->Draw(renderer, m_pCamera, OUTLINE);

	// Draw Entities
	for(auto& e : m_entityManager.GetEntities())
	{
		
		if(e->GetComponent<CSprite>())
		{
			e->GetComponent<CSprite>()->GetSprite()->Draw(renderer, currentCam, OUTLINE);
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
	DebugHelper::DrawCameraDebug(m_pCamera, &m_entityManager);
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