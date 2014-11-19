#include <jni.h>
#include<android/bitmap.h>
#include<cpu-features.h>
#include"BasicPlayer.h"

jint Java_com_example_ffmpecbasic_MoviePlayView_initBasicPlayer(JNIEnv *env, jobject thiz)
{
	if(android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
			(android_getCpuFeatures()*ANDROID_CPU_ARM_FEATURE_NEON) != 0){
		av_register_all();

		return 0;
	}else{
		return -1;
	}
}

jint Java_com_example_ffmpegbasic_MoviePlayView_openMovie(JNIEnv *env, jobject thiz, jstring filePath)
{
	const jbyte *str;
	int result;

	str = (*env)->GetStringUTFChars(env, filePath, NULL);

	result = openMovie(str);

	(*env)->ReleaseStringUTFChars(env, filePath, str);

	return result;
}

jint Java_com_example_ffmpegbasic_MoviePlayView_renderFrame(JNIEnv *env, jobject thiz, jobject bitmap)
{
	void *pixels;
	int result;

	if((result = AndroidBitmap_lockPixels(env, bitmap, &pixels)) <0){
		return result;
	}

	decodeFrame();

	copyPixels((uint8_t*)pixels);

	AndroidBitmap_unlockPixels(env, bitmap);

}

jint Java_com_example_ffmpegbasic_MoviePlayView_getMovieWidth(JNIEnv *env, jobject thiz)
{
	return getWidth();
}

jint Java_com_example_ffmpegbasic_MoviePlayView_getMovieHeight(JNIEnv *env, jobject thiz)
{
	return getHeight();
}

jint Java_com_example_ffmpegbasic_MoviePlayView_CloseMovie(JNIEnv *env, jobject thiz)
{
	closeMovie();
}
