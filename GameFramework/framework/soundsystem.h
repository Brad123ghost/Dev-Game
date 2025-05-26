// COMP710 GP Framework 2024
#ifndef __SOUNDSYSTEM_H_
#define __SOUNDSYSTEM_H_

#include "fmod.hpp"

#include <string>
#include <map>

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void CreateSystem();
	void Initialize();
	void Release();
	void CreateSound(const char* key, const char* pcSoundFileName);
	void PlaySound(const char* key);
	void Update();

protected:

private:

public:

protected:
	FMOD_RESULT m_frResult;
	FMOD::System* m_pSystem;
	std::map<std::string, FMOD::Sound*> m_pLoadedSounds;

private:

};

#endif // __SOUNDSYSTEM_H_
