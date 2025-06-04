#include "debughelper.h"
#include "camera.h"
#include "imgui.h"
#include "entitymanager.h"

void DebugHelper::DrawCameraDebug(Camera* pCamera, EntityManager* pEntityManager)
{
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		bool camChecked = pCamera->IsEnabled();
		ImGui::Checkbox("Camera Enabled", &camChecked);
		if(camChecked)
		{
			pCamera->Enable();
		}
		else
		{
			pCamera->Disable();
		}

		if (pCamera->IsEnabled())
		{
			Camera* currentCam = pCamera;
			const char* camTypeItems[] = { "Free Cam", "Follow" };
			static int item_selected_idx = 0;
			const char* combo_preview_value = camTypeItems[item_selected_idx];
			ImGui::SeparatorText("Camera Type");
			if (ImGui::BeginCombo("##Camera_Type", combo_preview_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(camTypeItems); n++)
				{
					const bool is_selected = (item_selected_idx == n); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(camTypeItems[n], is_selected))
					{
						item_selected_idx = n;
						currentCam->SetCameraType(static_cast<CameraType>(n));
					}
				}
				ImGui::EndCombo();
			}
			if (item_selected_idx != 1)
			{
				int camSpeed = currentCam->GetCamSpeed()/10;
				if (ImGui::SliderInt("Move Speed", &camSpeed, 0, 60))
					currentCam->SetCamSpeed(camSpeed);

				ImGui::Text("Current Cam Speed: %d", camSpeed);
			} 
			else // If not free cam display list of objects to follow (Players only for now!)
			{
				EntityVec players = pEntityManager->GetEntities(eTag::PLAYER);
				static int selectedPlayerIndex = 0;
				std::string temp = players[selectedPlayerIndex]->GetTagString();
				const char* playerComboPreviewValue = temp.c_str();;

				if (ImGui::BeginCombo("##Player list", playerComboPreviewValue))
				{
					for (int i = 0; i < players.size(); i++)
					{
						const bool is_selected = (selectedPlayerIndex == i);
						std::string name = "Player##" + i;
						
						if (ImGui::Selectable(name.c_str(), is_selected))
						{
							selectedPlayerIndex = i;
						}
					}
					ImGui::EndCombo();
				}
				Vector2 playerPos = players[selectedPlayerIndex]->GetComponent<CTransform>()->position;
				ImGui::Text("World Position: (%.2f, %.2f)", playerPos.x, playerPos.y);
				currentCam->SetPosition(static_cast<int>(playerPos.x), static_cast<int>(playerPos.y));
			}
			/*Vector2 moveVector
			if (moveVector.x != 0 && moveVector.y != 0) {
				moveVector.Normalize();
			}
			ImGui::Text("Vector (%.3f, %.3f)", moveVector.x, moveVector.y);*/
			ImGui::SeparatorText("Camera Position");
			Vector2 cameraPos = currentCam->GetPosition();
			ImGui::Text("X: %f", cameraPos.x);
			ImGui::Text("Y: %f", cameraPos.y);
			ImGui::Text("Zoom: %f", currentCam->GetZoom());
			if (ImGui::Button("Reset Camera"))
			{
				currentCam->ResetCamera();
			}
		}
	}
}

void DebugHelper::DrawGridDebug(int& gSize, int& cSize, bool& showGrid)
{
	int step = 2;
	if (ImGui::CollapsingHeader("Grid Settings"))
	{
		ImGui::Text("Grid Size: %d", gSize);
		ImGui::Text("Cell Size: %d", cSize);
		ImGui::Checkbox("Show Grid", &showGrid);
		if (!showGrid)
			ImGui::BeginDisabled();
		ImGui::SliderInt("Grid Size", &gSize, 0, 20);
		
		ImGui::SliderInt("Cell Size", &cSize, 0, 100);

		if (!showGrid)
			ImGui::EndDisabled();
		/*if (ImGui::Button("Generate Grid"))
			m_pRenderer->GenerateGrid(m_iGridSize, m_iCellSize);*/
	}
}