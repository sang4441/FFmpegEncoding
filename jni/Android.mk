LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libbasicplayer
LOCAL_CFLAGS := -DANDROID_NDK
LOCAL_SRC_FILES := BasicPlayer.c Interface.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/ \
					$(LOCAL_PATH)/ffmpeg/libavcodec \
					$(LOCAL_PATH)/ffmpeg/libavformat \
					$(LOCAL_PATH)/ffmpeg/libswscale

LOCAL_STATIC_LIBRARIES := libavformat libavcodec libswscale libavutil cpufeatures
LOCAL_LDLIBS := -L$(LOCAL_PATH)/ffmpeg/android/armv7-a/lib -lavformat -lavcodec -lswscale -lavutil -lz -ljnigraphics
LOCAL_ARM_MODE := arm

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/cpufeatures)


