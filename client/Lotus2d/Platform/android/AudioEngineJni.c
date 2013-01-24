/********************************************************************
	created:	2012/09/20
	filename: 	AudioEngineJni.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "Platform/android/Android.h"
#include "Base/Log.h"

#ifndef int
#define int int
#endif

void preloadBackgroundMusicJNI(const char *path)
{
	#if 0
// void playBackgroundMusic(String,boolean)
	JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "preloadBackgroundMusic", "(Ljava/lang/String;)V"))
	{			
		return;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(path);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
	methodInfo.env->DeleteLocalRef(stringArg);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void playBackgroundMusicJNI(const char *path, int isLoop)
{
	JNIEnv* env;
	LOGD("playBackgroundMusicJNI","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	jclass classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	jmethodID methodId = (*env)->GetStaticMethodID(env,classAndroid,"playBackgroundMusic", "(Ljava/lang/String;Z)V");
	
	jstring StringArg = (*env)->NewStringUTF(env,path);
	(*env)->CallStaticVoidMethod(env,classAndroid, methodId,StringArg,isLoop);
	
	(*env)->PopLocalFrame(env, 0);

	LOGD("playBackgroundMusicJNI OK path= %s", path);

}

void stopBackgroundMusicJNI()
{
	jclass classAndroid;
	JNIEnv* env;
	LOGD("playBackgroundMusicJNI","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	jmethodID methodId =(*env)->GetStaticMethodID(env,classAndroid,"stopBackgroundMusic", "()V");
	(*env)->CallStaticVoidMethod(env,classAndroid, methodId);
	(*env)->PopLocalFrame(env, 0);

	LOGD("stopBackgroundMusic %s","OK");

}

void pauseBackgroundMusicJNI()
{
	// void pauseBackgroundMusic()
#if 0

	JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "pauseBackgroundMusic", "()V"))
	{
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void resumeBackgroundMusicJNI()
{
	// void resumeBackgroundMusic()

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "resumeBackgroundMusic", "()V"))
	{
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void rewindBackgroundMusicJNI()
{
	// void rewindBackgroundMusic()
#if 0

	JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "rewindBackgroundMusic", "()V"))
	{
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

int isBackgroundMusicPlayingJNI()
{
	// boolean rewindBackgroundMusic()

#if 0
JniMethodInfo methodInfo;
	jboolean ret = false;

	if (! getStaticMethodInfo(methodInfo, "isBackgroundMusicPlaying", "()Z"))
	{
		return ret;
	}

	ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	return ret;
#endif
}

float getBackgroundMusicVolumeJNI()
{
	// float getBackgroundMusicVolume()

#if 0
JniMethodInfo methodInfo;
	jfloat ret = -1.0;

	if (! getStaticMethodInfo(methodInfo, "getBackgroundMusicVolume", "()F"))
	{
		return ret;
	}

	ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	return ret;
#endif
}

void setBackgroundMusicVolumeJNI(float volume)
{
	// void setBackgroundMusicVolume()

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "setBackgroundMusicVolume", "(F)V"))
	{
		return ;
	}

    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

unsigned int playEffectJNI(const char* path, int bLoop)
{
	// int playEffect(String)

#if 0
	JniMethodInfo methodInfo;
	int ret = 0;

	if (! getStaticMethodInfo(methodInfo, "playEffect", "(Ljava/lang/String;Z)I"))
	{
		return ret;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(path);
	ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, stringArg, bLoop);
    methodInfo.env->DeleteLocalRef(stringArg);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	return (unsigned int)ret;
#endif
}

void stopEffectJNI(unsigned int nSoundId)
{
	// void stopEffect(int)

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "stopEffect", "(I)V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void endJNI()
{
	// void end()

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "end", "()V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

float getEffectsVolumeJNI()
{
	// float getEffectsVolume()

	#if 0
JniMethodInfo methodInfo;
	jfloat ret = -1.0;

	if (! getStaticMethodInfo(methodInfo, "getEffectsVolume", "()F"))
	{
		return ret;
	}

	ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	return ret;
#endif
}

void setEffectsVolumeJNI(float volume)
{
	// void setEffectsVolume(float)
#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "setEffectsVolume", "(F)V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void preloadEffectJNI(const char *path)
{
	// void preloadEffect(String)

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "preloadEffect", "(Ljava/lang/String;)V"))
	{
		return ;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(path);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
	methodInfo.env->DeleteLocalRef(stringArg);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void unloadEffectJNI(const char* path)
{
	// void unloadEffect(String)

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "unloadEffect", "(Ljava/lang/String;)V"))
	{
		return ;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(path);
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
	methodInfo.env->DeleteLocalRef(stringArg);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void pauseEffectJNI(unsigned int nSoundId)
{
	// void pauseEffect(int)

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "pauseEffect", "(I)V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void pauseAllEffectsJNI()
{
	// void pauseAllEffects()

	#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "pauseAllEffects", "()V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void resumeEffectJNI(unsigned int nSoundId)
{
	// void resumeEffect(int)

	#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "resumeEffect", "(I)V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void resumeAllEffectsJNI()
{
	// void resumeAllEffects()

#if 0
JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "resumeAllEffects", "()V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void stopAllEffectsJNI()
{
#if 0
// void stopAllEffects()

	JniMethodInfo methodInfo;

	if (! getStaticMethodInfo(methodInfo, "stopAllEffects", "()V"))
	{
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}
