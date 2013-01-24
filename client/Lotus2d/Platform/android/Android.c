/********************************************************************
	created:	2012/10/26
	filename: 	Android.c
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include <jni.h>
#include "Android.h"
#include "Base/Log.h"

JavaVM *g_JavaVM = 0;
jclass g_classAndroid = 0;


JNIEnv* getEnv()
{
	JNIEnv *env = 0;
	if(g_JavaVM){
		(*g_JavaVM)->GetEnv(g_JavaVM,(void**)&env, JNI_VERSION_1_4);
	}
	LOGD("getEnv","");
	return env;
}


jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	g_JavaVM = vm;
	return JNI_VERSION_1_4;
}

jmethodID getMethodID(const char *methodName, const char *paramCode)
{
	jmethodID ret = 0;
	// get jni environment and java class
	JNIEnv *env = getEnv();
	if (!env)
	{
		LOGD("Failed to get the environment using GetEnv()","");
		return 0;
	}

	if (env != 0 && g_classAndroid != 0)
	{
		LOGD("GetStaticMethodID name:%s",methodName);
		//ret = (*env)->GetStaticMethodID(env,g_classAndroid, methodName, paramCode);
		LOGD("GetStaticMethodID finish","");
	}

	if (! ret)
	{
		LOGD("get method id of %s error", methodName);
	}

	LOGD("GetStaticMethodID got","");
	return ret;
}


