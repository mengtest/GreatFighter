LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../../../DevEnv/cocos2d-x-3.4)
$(call import-add-path,$(LOCAL_PATH)/../../../../../../DevEnv/cocos2d-x-3.4/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../../../DevEnv/cocos2d-x-3.4/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Src/Core/AppDelegate.cpp \
                   ../../../Src/Scene/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Src

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
