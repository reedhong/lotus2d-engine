/********************************************************************
	created:	2012/11/26
	filename: 	textfield_android.c
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Platform/textfield.h"
#include "Android.h"
#include <jni.h>
#include "Platform/android/Android.h"
#include "Base/Log.h"

IInputEditInterface g_editor_interface;

void	android_editor_setVisiable(BOOL visible)
{
	jclass classAndroid;
	jmethodID setEditorMethodID;
	JNIEnv* env;
	LOGD("android_editor_setVisiable","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorVisibility, "(Z)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,visible);
	}
	(*env)->PopLocalFrame(env, 0);

}
void	android_editor_setExtent(int x, int y, int w, int h)
{
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	LOGD("android_editor_setExtent","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorExtent, "(IIII)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,x,y,w,h);
	}
	(*env)->PopLocalFrame(env, 0);

}

void	android_editor_setText(const char* text)
{
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	if(text){
		LOGD("android_editor_setText","");
		(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
		(*env)->PushLocalFrame(env, 2);
		classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
		if(classAndroid){
			jstring StringArg = (*env)->NewStringUTF(env,text);
			setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorText, "(Ljava/lang/String;)V");
			(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,StringArg);
		}
		(*env)->PopLocalFrame(env, 0);
	}
}


char*	android_editor_getText()
{
	char* sRet = 0;
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	LOGD("android_editor_getText","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_getEditorText, "()Ljava/lang/String;");
		jstring text = (jstring) (*env)->CallStaticObjectMethod(env,classAndroid, setEditorMethodID);
		// todo: 是否需要copy出来？
		char* tmp=(char *)(*env)->GetStringUTFChars(env,text,JNI_FALSE);
		LOGD("android_editor_getText %s", tmp);
		int strLen = strlen(tmp);
		sRet = (char *)malloc(strLen+1);
		memcpy(sRet, tmp, strLen);
		sRet[strLen] = '\0';
	}
	(*env)->PopLocalFrame(env, 0);
	return sRet;
}

void	android_editor_setInputMethod(Textfield_Input_Method mode)
{
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	LOGD("android_editor_setInputMethod","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorInputMode, "(I)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,mode);
	}
	(*env)->PopLocalFrame(env, 0);
}

void	android_editor_setInputPassword(BOOL isPassword)
{
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	LOGD("android_editor_setInputPassword","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorPassword, "(Z)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,isPassword);
	}
	(*env)->PopLocalFrame(env, 0);
}

void	android_editor_setInputMaxLength(int maxLength)
{

	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorMethodID;

	LOGD("android_editor_setInputMaxLength","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorMethodID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorMaxLength, "(I)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorMethodID,maxLength);
	}
	(*env)->PopLocalFrame(env, 0);

}

void android_editor_setInputColor(int argb)
{
	jclass classAndroid;

	JNIEnv* env;
	jmethodID setEditorColorID;

	LOGD("android_editor_setInputColor","");
	(*g_JavaVM)->AttachCurrentThread(g_JavaVM, &env, 0);
	(*env)->PushLocalFrame(env, 2);
	classAndroid = (*env)->FindClass(env,AdapterAndroidClass);
	if(classAndroid)
	{
		setEditorColorID = (*env)->GetStaticMethodID(env,classAndroid, AdapterAndroidClass_Method_setEditorTextColor, "(I)V");
		(*env)->CallStaticVoidMethod(env,classAndroid, setEditorColorID,argb);
	}
	(*env)->PopLocalFrame(env, 0);
}

void lotus2d_edit_init(void* pEditor)
{
	LOGD("android lotus2d_edit_init","");

	g_inputEditor.pEditor = pEditor;
	g_editor_interface._setText = android_editor_setText;
	g_editor_interface._getText = android_editor_getText;
	g_editor_interface._setExtent = android_editor_setExtent;
	g_editor_interface._setVisiable = android_editor_setVisiable;
	g_editor_interface._setInputMethod = android_editor_setInputMethod;
	g_editor_interface._setInputPassword = android_editor_setInputPassword;
	g_editor_interface._setInputMaxLength = android_editor_setInputMaxLength;
	g_editor_interface._setInputColor = android_editor_setInputColor;
	g_inputEditor.delegate = &g_editor_interface;
}