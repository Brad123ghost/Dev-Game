#include "soundsystem.h"

#include "fmod.hpp"
#include <assert.h>
SoundSystem::SoundSystem()
	: m_frResult()
	, m_pSystem(0)
{

}

SoundSystem::~SoundSystem()
{
	m_pLoadedSounds.clear();
}

void SoundSystem::CreateSound(const char* key, const char* pcSoundFileName)
{
	m_pSystem->createSound(pcSoundFileName, FMOD_DEFAULT, NULL, &m_pLoadedSounds[key]);
}

void SoundSystem::PlaySound(const char* key)
{
	m_pSystem->playSound(m_pLoadedSounds[key], NULL, false, NULL);
}

void SoundSystem::Update()
{
	m_pSystem->update();
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