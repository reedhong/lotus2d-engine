/********************************************************************
	created:	2012/09/20
	filename: 	AudioEngine.cpp
	author:		reedhong
	
	purpose:	win32 audio
*********************************************************************/
#include "Audio/AudioEngine.h"

namespace Lotus2d {


	AudioEngine::AudioEngine():mMusic(0)
	{
		FSOUND_Init(44100,32,0);
	}

	AudioEngine::~AudioEngine()
	{
	}

	AudioEngine* AudioEngine::Instance()
	{
		static AudioEngine s_instance;
		return &s_instance;
	}

	void AudioEngine::end()
	{
	
	}

	void AudioEngine::setResource(const char* pszZipFileName)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// BackgroundMusic
	//////////////////////////////////////////////////////////////////////////

	void AudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
		if(mMusic){
			stopBackgroundMusic(0);
			FSOUND_Sample_Free((mMusic));
		}
		mMusic = FSOUND_Sample_Load(0, pszFilePath, 0, 0,0);
		if(mMusic){
			FSOUND_PlaySound(0, mMusic);
			FSOUND_SetVolume(0, 255);
			if(bLoop){
				FSOUND_SetLoopMode(0, FSOUND_LOOP_NORMAL);
			}else{
				FSOUND_SetLoopMode(0, FSOUND_LOOP_OFF);
			}
		}
	}

	void AudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
		FSOUND_StopSound(0);
	}

	void AudioEngine::pauseBackgroundMusic()
	{
		FSOUND_SetPaused(0, true);
	}

	void AudioEngine::resumeBackgroundMusic()
	{
		FSOUND_SetPaused(0, false);
	}

	void AudioEngine::rewindBackgroundMusic()
	{

	}

	bool AudioEngine::willPlayBackgroundMusic()
	{
		return false;
	}

	bool AudioEngine::isBackgroundMusicPlaying()
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// effect function
	//////////////////////////////////////////////////////////////////////////

	unsigned int AudioEngine::playEffect(const char* pszFilePath, bool bLoop)
	{
		return 0;
	}

	void AudioEngine::stopEffect(unsigned int nSoundId)
	{

	}

	void AudioEngine::preloadEffect(const char* pszFilePath)
	{
		
	}

	void AudioEngine::pauseEffect(unsigned int nSoundId)
	{
	
	}

	void AudioEngine::pauseAllEffects()
	{

	}

	void AudioEngine::resumeEffect(unsigned int nSoundId)
	{

	}

	void AudioEngine::resumeAllEffects()
	{

	}

	void AudioEngine::stopAllEffects()
	{

	}

	void AudioEngine::preloadBackgroundMusic(const char* pszFilePath)
	{

	}

	void AudioEngine::unloadEffect(const char* pszFilePath)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// volume interface
	//////////////////////////////////////////////////////////////////////////

	float AudioEngine::getBackgroundMusicVolume()
	{
		return 1.0;
	}

	void AudioEngine::setBackgroundMusicVolume(float volume)
	{
	}

	float AudioEngine::getEffectsVolume()
	{
		return 1.0;
	}

	void AudioEngine::setEffectsVolume(float volume)
	{
		FSOUND_SetVolume(0,(int)(255*volume));
	}

	
} // end of namespace Lotus2d
