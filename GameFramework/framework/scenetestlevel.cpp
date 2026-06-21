#include "scenetestlevel.h"

#include "renderer.h"
#include "imgui.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "camera.h"
#include "debughelper.h"
#include "texture.h"
#include "texturemanager.h"
#include "logmanager.h"
#include "sprocessinput.h"
#include "sprocesscollider.h"
#include "animator.h"
#include "game.h"
#include "sanimator.h"
#include "soundsystem.h"

SceneTestLevel::SceneTestLevel()
	: m_bShowGrid(false)
	, m_bDrawAABB(false)
	, m_pTestSprite(0)
	, m_pCamera(0)
	, gridSize(10)
	, cellSize(32)
	, m_pRenderer(0)
	, m_pAnimator(0)
	, coins(0)
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

	delete m_pCoinSpin;
	m_pCoinSpin = 0;

	delete m_pAnimator;
	m_pAnimator = 0;

	/*delete m_pSAnimator;
	m_pSAnimator = 0;*/

}
void coinPickupPlay()
{
	std::cout << "Sound play" << std::endl;
}
bool SceneTestLevel::Initialize(Renderer& renderer, SoundSystem& soundSystem)
{
	InputSystem& inputSystem = Game::GetInstance().GetInputSystem();
	m_pSoundSystem = &soundSystem;

	m_pAnimator = new Animator();
	AnimatedSprite* pIdleLeft = renderer.CreateAnimatedSprite("sprites\\idle_left.png");
	pIdleLeft->SetPos(0, 0);
	pIdleLeft->SetupFrames(64, 64);
	pIdleLeft->SetFrameDuration(0.1f);
	pIdleLeft->SetLooping(true);
	pIdleLeft->Animate();
	m_pAnimator->AddAnimation("IdleLeft", pIdleLeft);
	AnimatedSprite* pIdleRight = renderer.CreateAnimatedSprite("sprites\\idle_right.png");
	pIdleRight->SetPos(0, 0);
	pIdleRight->SetupFrames(64, 64);
	pIdleRight->SetFrameDuration(0.1f);
	pIdleRight->SetLooping(true);
	pIdleRight->Animate();
	m_pAnimator->AddAnimation("IdleRight", pIdleRight);
	AnimatedSprite* pWalkLeft = renderer.CreateAnimatedSprite("sprites\\walking_left.png");
	pWalkLeft->SetPos(0, 0);
	pWalkLeft->SetupFrames(64, 64);
	pWalkLeft->SetFrameDuration(0.1f);
	pWalkLeft->SetLooping(true);
	pWalkLeft->Animate();
	m_pAnimator->AddAnimation("WalkLeft", pWalkLeft);
	AnimatedSprite* pWalkRight = renderer.CreateAnimatedSprite("sprites\\walking_right.png");
	pWalkRight->SetPos(0, 0);
	pWalkRight->SetupFrames(64, 64);
	pWalkRight->SetFrameDuration(0.1f);
	pWalkRight->SetLooping(true);
	pWalkRight->Animate();
	m_pAnimator->AddAnimation("WalkRight", pWalkRight);
	AnimatedSprite* pRuninngLeft = renderer.CreateAnimatedSprite("sprites\\running_left.png");
	pRuninngLeft->SetPos(0, 0);
	pRuninngLeft->SetupFrames(64, 64);
	pRuninngLeft->SetFrameDuration(0.1f);
	pRuninngLeft->SetLooping(true);
	pRuninngLeft->Animate();
	m_pAnimator->AddAnimation("RunLeft", pRuninngLeft);
	AnimatedSprite* pRuninngRight = renderer.CreateAnimatedSprite("sprites\\running_right.png");
	pRuninngRight->SetPos(0, 0);
	pRuninngRight->SetupFrames(64, 64);
	pRuninngRight->SetFrameDuration(0.1f);
	pRuninngRight->SetLooping(true);
	pRuninngRight->Animate();
	m_pAnimator->AddAnimation("RunRight", pRuninngRight);

	m_pAnimator->SetDefaultState("Idle");
	m_pAnimator->SetPosition(300, 300);

	// New Player with Animator
	std::shared_ptr<NewEntity> player0 = m_entityManager.CreateEntity("Player", eTag::PLAYER);
	player0->AddComponent<CTransform>(Vector2(600,600));
	player0->GetComponent<CTransform>()->facing = eFacing::LEFT;
	player0->AddComponent<CInput>();
	player0->AddComponent<CCollider>(Vector2(22, 36));
	player0->AddComponent<CAnimator>();
	player0->GetComponent<CAnimator>()->m_animations.insert({"IdleLeft", pIdleLeft});
	player0->GetComponent<CAnimator>()->m_animations.insert({"IdleRight", pIdleRight });
	player0->GetComponent<CAnimator>()->m_animations.insert({"WalkLeft", pWalkLeft });
	player0->GetComponent<CAnimator>()->m_animations.insert({"WalkRight", pWalkRight });
	player0->GetComponent<CAnimator>()->m_animations.insert({"RunLeft", pRuninngLeft });
	player0->GetComponent<CAnimator>()->m_animations.insert({"RunRight", pRuninngRight });
	player0->GetComponent<CAnimator>()->m_sActiveState = "Idle";
	
	InputMode mode = player0->GetComponent<CInput>()->m_eInputMode;
	CInput* playerInput = player0->GetComponent<CInput>();
	// Animation Condition based on player input values set in cinput
	// Todo Fix so when opposite key is pressed the animation goes back to default
	// Idle to Run
	AnimationTransition2 idleRunTransition{ "Run",  [playerInput]() {
			return (playerInput->m_bShift && (playerInput->m_bUp || playerInput->m_bDown || playerInput->m_bLeft || playerInput->m_bRight));
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Idle"].push_back(idleRunTransition);

	//Idle to walk
	AnimationTransition2 idleWalkTransition{ "Walk", [playerInput]() {
			return (playerInput->m_bUp || playerInput->m_bDown || playerInput->m_bLeft || playerInput->m_bRight);
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Idle"].push_back(idleWalkTransition);

	// Walk to run
	AnimationTransition2 walkRunTransition{ "Run", [playerInput]() {
			return (playerInput->m_bShift && (playerInput->m_bUp || playerInput->m_bDown || playerInput->m_bLeft || playerInput->m_bRight));
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Walk"].push_back(walkRunTransition);

	// Run to walk
	AnimationTransition2 runWalkTransition{ "Walk", [playerInput]() {
			return (!playerInput->m_bShift && (playerInput->m_bUp || playerInput->m_bDown || playerInput->m_bLeft || playerInput->m_bRight));
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Run"].push_back(runWalkTransition);

	// Walk to idle
	AnimationTransition2 walkIdleTransition{ "Idle", [playerInput]() {
			return (!playerInput->m_bUp && !playerInput->m_bDown && !playerInput->m_bLeft && !playerInput->m_bRight);
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Walk"].push_back(walkIdleTransition);

	// Run to idle
	AnimationTransition2 runIdleTransition{ "Idle", [playerInput]() {
			return (playerInput->m_bShift && !playerInput->m_bUp && !playerInput->m_bDown && !playerInput->m_bLeft && !playerInput->m_bRight);
		} };
	player0->GetComponent<CAnimator>()->m_transitions["Run"].push_back(runIdleTransition);

	m_pRenderer = &renderer;
	m_pCamera = new Camera(renderer.GetWidth(), renderer.GetHeight());

	renderer.SetClearColor(0, 0, 0);
	m_pTestSprite = renderer.CreateSprite("sprites\\crate.png");
	m_pTestSprite->SetX(300);
	m_pTestSprite->SetY(300);
	m_pTestSprite->SetScale(0.5f);

	// Add player
	std::shared_ptr<NewEntity> player = m_entityManager.CreateEntity("Dummy 0", eTag::DEFAULT);
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
	sprite->Initialize(*renderer.CreateTexture("sprites\\ball.png"));
	player->AddComponent<CSprite>(sprite);
	player->AddComponent<CTransform>(Vector2(250, 250));

	std::shared_ptr<NewEntity> player2 = m_entityManager.CreateEntity("Dummy 1", eTag::DEFAULT);
	std::shared_ptr<Sprite> sprite2 = std::make_shared<Sprite>();
	sprite2->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player2->AddComponent<CSprite>(sprite2);
	player2->AddComponent<CTransform>(Vector2(-250.f, -250));

	std::shared_ptr<NewEntity> player3 = m_entityManager.CreateEntity("Dummy 2", eTag::DEFAULT);
	std::shared_ptr<Sprite> sprite3 = std::make_shared<Sprite>();
	sprite3->Initialize(*renderer.CreateTexture("sprites\\crate.png"));
	player3->AddComponent<CSprite>(sprite3);
	player3->AddComponent<CTransform>(Vector2(-300, 300));

	std::shared_ptr<NewEntity> coin = m_entityManager.CreateEntity("Coin", eTag::ITEM);
	coin->AddComponent<CTransform>(Vector2(500, 500));
	coin->AddComponent<CAnimator>();
	coin->AddComponent<CCollider>(Vector2(16, 16),true ,true);


	SoundSystem* sound = m_pSoundSystem;
	coin->GetComponent<CCollider>()->onEnterCondition = [sound, this](void) {
		sound->PlaySound("pickup-1");
		AddCoins(1);
		};
	m_pCoinSpin = renderer.CreateAnimatedSprite("sprites\\spin_gold_coin_strip.png");
	//std::shared_ptr<AnimatedSprite> pCoinSpin(renderer.CreateAnimatedSprite("sprites\\spin_gold_coin_strip.png"));
	m_pCoinSpin->SetupFrames(16, 16);
	m_pCoinSpin->SetPos(500, 500);
	m_pCoinSpin->SetFrameDuration(0.1f);
	m_pCoinSpin->SetLooping(true);
	m_pCoinSpin->Animate();
	coin->GetComponent<CAnimator>()->m_animations.insert({ "Spin", m_pCoinSpin });
	coin->GetComponent<CAnimator>()->m_sActiveState = "Spin";

	m_pSoundSystem->CreateSound("pickup-1", "audio\\pickup-1.wav");
	m_pSoundSystem->CreateSound("pickup-2", "audio\\pickup-2.wav");
	m_pSoundSystem->CreateSound("level-up-1", "audio\\level-up-1.wav");
	m_pSoundSystem->CreateSound("level-up-2", "audio\\level-up-2.wav");

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
	SProcessCollider::ProcessCollider(deltaTime, m_entityManager);
	// Process animator
	SAnimator::ProcessAnimator(deltaTime, &m_entityManager);
	
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

	/*std::string title = "Test Level - " + std::to_string(counter);
	renderer.DrawText(title.c_str(), 10, 10, 1.0f);*/

	std::string coinText = "Coins: " + std::to_string(coins);
	renderer.DrawText(coinText.c_str(), 10, 50, 1.0f, {0,0,0,1});
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
	m_pTestSprite->Draw(renderer, m_pCamera);

	// Draw Entities
	for(auto& e : m_entityManager.GetEntities())
	{
		
		if(e->GetComponent<CSprite>())
		{
			e->GetComponent<CSprite>()->GetSprite()->Draw(renderer, currentCam);
		}
	}

	SAnimator::DrawAnimator(renderer, &m_entityManager);

	if(m_bDrawAABB)
	{
		/*renderer.DrawAABB(0, 0, 2, 2);
		std::cout << "AABB Enabled" << std::endl;*/
		for (auto& e : m_entityManager.GetEntities())
		{
			CCollider* collider = e->GetComponent<CCollider>();
			CTransform* transform = e->GetComponent<CTransform>();
			if (collider && collider->isActive && transform)
			{
				// Calculate half size
				Vector2 halfSize = { collider->size.x / 2.0f, collider->size.y / 2.0f };
				Vector2 center = transform->position;

				// Calculate corners
				Vector2 topLeft = { center.x - halfSize.x, center.y - halfSize.y };
				Vector2 topRight = { center.x + halfSize.x, center.y - halfSize.y };
				Vector2 bottomRight = { center.x + halfSize.x, center.y + halfSize.y };
				Vector2 bottomLeft = { center.x - halfSize.x, center.y + halfSize.y };

				// Draw bounding box (assuming you have a color, e.g., Color::Red)
				renderer.DrawLine2D({ topLeft.x, topLeft.y }, { topRight.x, topRight.y });
				renderer.DrawLine2D({ topLeft.x, topLeft.y }, { bottomLeft.x, bottomLeft.y });
				renderer.DrawLine2D({ topRight.x, topRight.y }, { bottomRight.x, bottomRight.y });
				renderer.DrawLine2D({ bottomLeft.x, bottomLeft.y }, { bottomRight.x, bottomRight.y });

			}
		}

		renderer.DrawLineFlush();
	}


}

void SceneTestLevel::ShootBullet(Vector2 spawnPos, float dir)
{
	std::shared_ptr<NewEntity> bullet = m_entityManager.CreateEntity("Bullet", eTag::PROJECTILE);
	
}

void SceneTestLevel::AddCoins(int amount)
{
	coins += amount;
}

void SceneTestLevel::SceneInfoDraw()
{
	ImGui::Text("Scene: Test Level");
	ImGui::Checkbox("Draw AABB", &m_bDrawAABB);
	DebugHelper::DrawCameraDebug(m_pCamera, &m_entityManager);
	m_pAnimator->DrawDebug();
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

void SceneTestLevel::EntitySpawnerDebugDraw(bool& open)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.85f));
	if (ImGui::Begin("Entity Spanwer", &open))
	{
		auto loadedTextureKeys = m_pRenderer->GetTextureManager()->GetLoadedTextureKeys();
		ImGui::Text("Entity Manager Debug");
		static int spawnSelectedTextureIndex = 0;
		const char* spawn_combo_preview_value = loadedTextureKeys[spawnSelectedTextureIndex].c_str();
		m_entityManager.DrawComboPreview(loadedTextureKeys, spawn_combo_preview_value, &spawnSelectedTextureIndex);
		if (ImGui::Button("Spawn Coin"))
		{
			std::shared_ptr<NewEntity> coin = m_entityManager.CreateEntity("Coin", eTag::ITEM);
			coin->AddComponent<CTransform>(Vector2(500, 500));
			coin->AddComponent<CAnimator>();
			coin->AddComponent<CCollider>(Vector2(16, 16), true, true);
			SoundSystem* sound = m_pSoundSystem;
			coin->GetComponent<CCollider>()->onEnterCondition = [sound, this](void) {
				sound->PlaySound("pickup-1");
				AddCoins(1);
				};
			AnimatedSprite* m_pCoinSpin = m_pRenderer->CreateAnimatedSprite("sprites\\spin_gold_coin_strip.png");

			m_pCoinSpin->SetupFrames(16, 16);
			m_pCoinSpin->SetPos(500, 500);
			m_pCoinSpin->SetFrameDuration(0.1f);
			m_pCoinSpin->SetLooping(true);
			m_pCoinSpin->Animate();
			coin->GetComponent<CAnimator>()->m_animations.insert({ "Spin", m_pCoinSpin });
			coin->GetComponent<CAnimator>()->m_sActiveState = "Spin";
		}
	}
	ImGui::End();
	ImGui::PopStyleColor();
}