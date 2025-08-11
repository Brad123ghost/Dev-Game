#include "soundsystem.h"

#include "fmod.hpp"
#include "imgui.h"
#include <assert.h>
#include <iostream>
SoundSystem::SoundSystem()
	: m_frResult()
	, m_pSystem(0)
	, m_pLoadedSounds()
	//, m_pSoundLocations()
{

}

SoundSystem::~SoundSystem()
{
	m_pLoadedSounds.clear();

	//m_pSoundLocations.clear();
}

void SoundSystem::CreateSound(const char* key, const char* pcSoundFileName)
{

	m_pSystem->createSound(pcSoundFileName, FMOD_DEFAULT, NULL, &m_pLoadedSounds[key]);
	const char* fileLocation = pcSoundFileName;
	void* pointer = (void*)fileLocation;
	m_pLoadedSounds[key]->setUserData(pointer);
	//m_pSoundLocations.insert({ key, pcSoundFileName });
	//m_pSoundLocations[key] = pcSoundFileName;
}

void SoundSystem::PlaySound(const char* key)
{
	m_pSystem->playSound(m_pLoadedSounds[key], NULL, false, NULL);
}

void SoundSystem::Update()
{
	m_pSystem->update();
}

void SoundSystem::DebugDraw()
{
	ImGui::Text("Loaded Sounds: %d", m_pLoadedSounds.size());
	static std::string selectedSoundKey = "";
	if (selectedSoundKey == "")
		ImGui::BeginDisabled();

	if (ImGui::Button("Play Sound"))
	{
		PlaySound(selectedSoundKey.c_str());
	}

	if (selectedSoundKey == "")
	{
		ImGui::EndDisabled();
		if (ImGui::BeginItemTooltip())
		{
			ImGui::Text("Select a sound to play");
			ImGui::EndTooltip();
		}
	}
		
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
	ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoReorder;
	ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap;
	ImGui::BeginTable("Sound List", 3, flags);
	ImGui::TableSetupColumn("Name", columnFlags);
	ImGui::TableSetupColumn("Length", columnFlags);
	ImGui::TableSetupColumn("File Path", columnFlags);
	//ImGui::TableSetupColumn("Type", columnFlags);
	ImGui::TableHeadersRow();
	
	for (const auto& sound : m_pLoadedSounds)
	{
		ImGui::PushID(sound.first.c_str());
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		bool isSelected = (selectedSoundKey == sound.first.c_str());
		if (ImGui::Selectable("##row", isSelected, selectableFlags))
		{
			if (isSelected)
				selectedSoundKey = "";
			else
				selectedSoundKey = sound.first.c_str();
			
		}
		ImGui::SameLine();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("%s", sound.first.c_str());

		FMOD::Sound* pSound = sound.second;
	
		unsigned int length = 0;
		pSound->getLength(&length, FMOD_TIMEUNIT_MS);
		ImGui::TableSetColumnIndex(1);
		float seconds = length / 1000.0f;
		ImGui::Text("%.3fs", seconds);

		//const char* filePath = m_pSoundLocations[sound.first].c_str(); // FMOD does not provide a direct way to get the file path
		void* filePath;
		sound.second->getUserData(&filePath);
		ImGui::TableSetColumnIndex(2);
		ImGui::Text("%s", (const char*)filePath);

		/*ImGui::TableNextColumn();
		ImGui::Text("Sound");
		FMOD_SOUND_TYPE type; 
		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;
		pSound->getFormat(&type, &format, &channels, &bits);*/
		//std::cout << type << std::endl;
		ImGui::PopID();
	}
	ImGui::EndTable();
}

void SoundSystem::CreateSystem()
{
	FMOD::System_Create(&m_pSystem);

}

void SoundSystem::Initialize()
{
	m_pSystem->init(512, FMOD_INIT_NORMAL, 0);
}

void SoundSystem::Release()
{
	m_pSystem->release();
}