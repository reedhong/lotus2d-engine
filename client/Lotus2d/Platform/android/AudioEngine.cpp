/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "Audio/AudioEngine.h"
#include "AudioEngineJni.h"

namespace Lotus2d {

	AudioEngine::AudioEngine()
	{

	}

	AudioEngine::~AudioEngine()
	{

	}


	void AudioEngine::end()
	{
		endJNI();
	}

	void AudioEngine::setResource(const char* pszZipFileName)
	{

	}

    void AudioEngine::preloadBackgroundMusic(const char* pszFilePath)
	{
        preloadBackgroundMusicJNI(pszFilePath);
	}

	void AudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
        playBackgroundMusicJNI(pszFilePath, bLoop);
	}

	void AudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
		stopBackgroundMusicJNI();
	}

	void AudioEngine::pauseBackgroundMusic()
	{
		pauseBackgroundMusicJNI();
	}

	void AudioEngine::resumeBackgroundMusic()
	{
		resumeBackgroundMusicJNI();
	} 

	void AudioEngine::rewindBackgroundMusic()
	{
		rewindBackgroundMusicJNI();
	}

	bool AudioEngine::willPlayBackgroundMusic()
	{
		return true;
	}

	bool AudioEngine::isBackgroundMusicPlaying()
	{
		return isBackgroundMusicPlayingJNI();
	}

	float AudioEngine::getBackgroundMusicVolume()
	{
		return getBackgroundMusicVolumeJNI();
	}

	void AudioEngine::setBackgroundMusicVolume(float volume)
	{
		setBackgroundMusicVolumeJNI(volume);
	}

	float AudioEngine::getEffectsVolume()
	{
		return getEffectsVolumeJNI();
	}

	void AudioEngine::setEffectsVolume(float volume)
	{
		setEffectsVolumeJNI(volume);
	}

	unsigned int AudioEngine::playEffect(const char* pszFilePath, bool bLoop)
	{
		return playEffectJNI(pszFilePath, bLoop);
	}

	void AudioEngine::stopEffect(unsigned int nSoundId)
	{
		stopEffectJNI(nSoundId);
	}

	void AudioEngine::preloadEffect(const char* pszFilePath)
	{
		preloadEffectJNI(pszFilePath);
	}

	void AudioEngine::unloadEffect(const char* pszFilePath)
	{
		unloadEffectJNI(pszFilePath);
	}

	void AudioEngine::pauseEffect(unsigned int nSoundId)
	{
		pauseEffectJNI(nSoundId);
	}

	void AudioEngine::pauseAllEffects()
	{
		pauseAllEffectsJNI();
	}

	void AudioEngine::resumeEffect(unsigned int nSoundId)
	{
		resumeEffectJNI(nSoundId);
	}

	void AudioEngine::resumeAllEffects()
	{
		resumeAllEffectsJNI();
	}

	void AudioEngine::stopAllEffects()
	{
		stopAllEffectsJNI();
	}
}
