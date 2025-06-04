#include "sprocessinput.h"
#include "imgui.h"

void SProcessInput::ProcessPlayerInput(float dt, EntityManager& entityManager, InputSystem& inputSystem, const Camera& camera)
{
	for (auto& e : entityManager.GetEntities(eTag::PLAYER))
	{
		CInput* input = e->GetComponent<CInput>();
		CTransform* transform = e->GetComponent<CTransform>();
		if (input && transform)
		{
			bool inputMode = (input->m_eInputMode == InputMode::WASD);
			input->m_bUp = inputMode ? inputSystem.GetKeyState(SDL_SCANCODE_W) : inputSystem.GetKeyState(SDL_SCANCODE_UP);
			input->m_bDown = inputMode ? inputSystem.GetKeyState(SDL_SCANCODE_S) : inputSystem.GetKeyState(SDL_SCANCODE_DOWN);
			input->m_bLeft = inputMode ? inputSystem.GetKeyState(SDL_SCANCODE_A) : inputSystem.GetKeyState(SDL_SCANCODE_LEFT);
			input->m_bRight = inputMode ? inputSystem.GetKeyState(SDL_SCANCODE_D) : inputSystem.GetKeyState(SDL_SCANCODE_RIGHT);
			input->m_bSpace = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);
			input->m_bShift = inputSystem.GetKeyState(SDL_SCANCODE_LSHIFT);
			input->m_bLeftClick = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
			input->m_bRightClick = inputSystem.GetMouseButtonState(SDL_BUTTON_RIGHT);

			Vector2 dir = transform->dir;
			if (input->m_bUp) dir.y = -1;
			if (input->m_bDown) dir.y = 1;
			if (input->m_bLeft) dir.x = -1;
			if (input->m_bRight) dir.x = 1;

			if (dir.x != 0 && dir.y != 0)
				dir.Normalize();

			float moveSpeed = input->m_bShift ? input->m_fSprintSpeed : input->m_fWalkSpeed;

			// Update Screen position
			transform->position.x += dir.x * moveSpeed * dt;
			transform->position.y += dir.y * moveSpeed * dt;
		}
	}
}

void SProcessInput::DrawDebug(CInput& cInput)
{
	int inputMode = static_cast<int>(cInput.m_eInputMode);
	ImGui::RadioButton("WASD", &inputMode, 0); ImGui::SameLine();
	ImGui::RadioButton("Arrow Keys", &inputMode, 1);
	if (inputMode != static_cast<int>(cInput.m_eInputMode))
		cInput.m_eInputMode = static_cast<InputMode>(inputMode);

	int walkSpeed = cInput.m_fWalkSpeed / 10;
	if(ImGui::SliderInt("Walk Speed", &walkSpeed, 0, 100))
		cInput.m_fWalkSpeed = walkSpeed * 10;

	int sprintSpeed = cInput.m_fSprintSpeed / 10;
	if (ImGui::SliderInt("Sprint Speed", &sprintSpeed, 0, 100))
		cInput.m_fSprintSpeed = sprintSpeed * 10;

	/*ImGui::Indent();
	if (ImGui::CollapsingHeader("Key States"))
	{
		ImGui::Text("Up: %s", cInput.m_bUp ? "Pressed" : "Released");
		ImGui::Text("Down: %s", cInput.m_bDown ? "Pressed" : "Released");
		ImGui::Text("Left: %s", cInput.m_bLeft ? "Pressed" : "Released");
		ImGui::Text("Right: %s", cInput.m_bRight ? "Pressed" : "Released");
		ImGui::Text("Space: %s", cInput.m_bSpace ? "Pressed" : "Released");
		ImGui::Text("Shift: %s", cInput.m_bShift ? "Pressed" : "Released");
		ImGui::Text("Left Click: %s", cInput.m_bLeftClick ? "Pressed" : "Released");
		ImGui::Text("Right Click: %s", cInput.m_bRightClick ? "Pressed" : "Released");
		ImGui::Unindent();
	}*/

}
