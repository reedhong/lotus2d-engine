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
#include "AudioEngine_objc.h"

static void static_end()
{
    [SimpleAudioEngine  end];
}

static void static_preloadBackgroundMusic(const char* pszFilePath)
{
    [[SimpleAudioEngine sharedEngine] preloadBackgroundMusic: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    [[SimpleAudioEngine sharedEngine] playBackgroundMusic: [NSString stringWithUTF8String: pszFilePath] loop: bLoop];
}

static void static_stopBackgroundMusic()
{
    [[SimpleAudioEngine sharedEngine] stopBackgroundMusic];
}

static void static_pauseBackgroundMusic()
{
     [[SimpleAudioEngine sharedEngine] pauseBackgroundMusic];
}

static void static_resumeBackgroundMusic()
{
    [[SimpleAudioEngine sharedEngine] resumeBackgroundMusic];
} 

static void static_rewindBackgroundMusic()
{
    [[SimpleAudioEngine sharedEngine] rewindBackgroundMusic];
}

static bool static_willPlayBackgroundMusic()
{
    return [[SimpleAudioEngine sharedEngine] willPlayBackgroundMusic];
}

static bool static_isBackgroundMusicPlaying()
{
    return [[SimpleAudioEngine sharedEngine] isBackgroundMusicPlaying];
}

static float static_getBackgroundMusicVolume()
{
    return [[SimpleAudioEngine sharedEngine] backgroundMusicVolume];
}

static void static_setBackgroundMusicVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [SimpleAudioEngine sharedEngine].backgroundMusicVolume = volume;
}
     
static float static_getEffectsVolume()
{
    return [[SimpleAudioEngine sharedEngine] effectsVolume];
}
     
static void static_setEffectsVolume(float volume)
{
    volume = MAX( MIN(volume, 1.0), 0 );
    [SimpleAudioEngine sharedEngine].effectsVolume = volume;
}
     
static unsigned int static_playEffect(const char* pszFilePath, bool bLoop)
{
    return [[SimpleAudioEngine sharedEngine] playEffect:[NSString stringWithUTF8String: pszFilePath] loop:bLoop]; 
}
     
static void static_stopEffect(int nSoundId)
{
    [[SimpleAudioEngine sharedEngine] stopEffect: nSoundId];
}
     
static void static_preloadEffect(const char* pszFilePath)
{
    [[SimpleAudioEngine sharedEngine] preloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}
     
static void static_unloadEffect(const char* pszFilePath)
{
    [[SimpleAudioEngine sharedEngine] unloadEffect: [NSString stringWithUTF8String: pszFilePath]];
}

static void static_pauseEffect(unsigned int uSoundId)
{
    [[SimpleAudioEngine sharedEngine] pauseEffect: uSoundId];
}

static void static_pauseAllEffects()
{
    [[SimpleAudioEngine sharedEngine] pauseAllEffects];
}

static void static_resumeEffect(unsigned int uSoundId)
{
    [[SimpleAudioEngine sharedEngine] resumeEffect: uSoundId];
}

static void static_resumeAllEffects()
{
    [[SimpleAudioEngine sharedEngine] resumeAllEffects];
}

static void static_stopAllEffects()
{
    [[SimpleAudioEngine sharedEngine] stopAllEffects];
}

namespace Lotus2d
{

	AudioEngine::AudioEngine()
	{

	}

	AudioEngine::~AudioEngine()
	{

	}


	void AudioEngine::end()
	{
        static_end();
	}

	void AudioEngine::setResource(const char* pszZipFileName)
	{

	}
    
    void AudioEngine::preloadBackgroundMusic(const char* pszFilePath)
	{
		static_preloadBackgroundMusic(pszFilePath);
	}

	void AudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
		static_playBackgroundMusic(pszFilePath, bLoop);
	}

	void AudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
        static_stopBackgroundMusic();
	}

	void AudioEngine::pauseBackgroundMusic()
	{
        static_pauseBackgroundMusic();
	}

	void AudioEngine::resumeBackgroundMusic()
	{
        static_resumeBackgroundMusic();
	} 

	void AudioEngine::rewindBackgroundMusic()
	{
        static_rewindBackgroundMusic();
	}

	bool AudioEngine::willPlayBackgroundMusic()
	{
        return static_willPlayBackgroundMusic();
	}

	bool AudioEngine::isBackgroundMusicPlaying()
	{
        return static_isBackgroundMusicPlaying();
	}

	float AudioEngine::getBackgroundMusicVolume()
	{
        return static_getBackgroundMusicVolume();
	}

	void AudioEngine::setBackgroundMusicVolume(float volume)
	{
        static_setBackgroundMusicVolume(volume);
	}

	float AudioEngine::getEffectsVolume()
	{
        return static_getEffectsVolume();
	}

	void AudioEngine::setEffectsVolume(float volume)
	{
        static_setEffectsVolume(volume);
	}

	unsigned int AudioEngine::playEffect(const char* pszFilePath, bool bLoop)
	{
        return static_playEffect(pszFilePath, bLoop);
	}

	void AudioEngine::stopEffect(unsigned int nSoundId)
	{
        static_stopEffect(nSoundId);
	}

	void AudioEngine::preloadEffect(const char* pszFilePath)
	{
        static_preloadEffect(pszFilePath);
	}

	void AudioEngine::unloadEffect(const char* pszFilePath)
	{
        static_unloadEffect(pszFilePath);
	}
	
	void AudioEngine::pauseEffect(unsigned int uSoundId)
	{
	    static_pauseEffect(uSoundId);
	}
	
	void AudioEngine::resumeEffect(unsigned int uSoundId)
	{
	    static_resumeEffect(uSoundId);
	}
	
	void AudioEngine::pauseAllEffects()
	{
	    static_pauseAllEffects();
	}
	
	void AudioEngine::resumeAllEffects()
	{
	    static_resumeAllEffects();
	}
	
	void AudioEngine::stopAllEffects()
	{
	    static_stopAllEffects();
	}
}