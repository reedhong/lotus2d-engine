/********************************************************************
	created:	2012/09/20
	filename: 	AudioEngineJni.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_AudioEngine_JNI_H__
#define __Lotus2d_AudioEngine_JNI_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern void preloadBackgroundMusicJNI(const char *path);
    extern void playBackgroundMusicJNI(const char *path, int isLoop);
	extern void stopBackgroundMusicJNI();
	extern void pauseBackgroundMusicJNI();
	extern void resumeBackgroundMusicJNI();
	extern void rewindBackgroundMusicJNI();
	extern int isBackgroundMusicPlayingJNI();
	extern float getBackgroundMusicVolumeJNI();
	extern void setBackgroundMusicVolumeJNI(float volume);
	extern unsigned int playEffectJNI(const char* path, int bLoop);
	extern void stopEffectJNI(unsigned int nSoundId);
	extern void endJNI();
	extern float getEffectsVolumeJNI();
	extern void setEffectsVolumeJNI(float volume);
	extern void preloadEffectJNI(const char *path);
	extern void unloadEffectJNI(const char* path);
	extern void pauseEffectJNI(unsigned int nSoundId);
	extern void pauseAllEffectsJNI();
	extern void resumeEffectJNI(unsigned int nSoundId);
	extern void resumeAllEffectsJNI();
	extern void stopAllEffectsJNI();
#ifdef __cplusplus
}
#endif

#endif // __SIMPLE_AUDIO_ENGINE_JNI__