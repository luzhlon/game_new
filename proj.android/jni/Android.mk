LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/extension)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/editor-support)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Client.cpp \
                   ../../Classes/Dialog.cpp \
                   ../../Classes/DrawNode3D.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/HelpScene.cpp \
                   ../../Classes/Man2Soldier.cpp \
                   ../../Classes/ManSoldier.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/NetRoom.cpp \
                   ../../Classes/ODSocket.cpp \
                   ../../Classes/Player.cpp \
                   ../../Classes/RoleScene.cpp \
                   ../../Classes/RoomListScene.cpp \
                   ../../Classes/RoomScene.cpp \
                   ../../Classes/SceneLayer.cpp \
                   ../../Classes/SettingScene.cpp \
                   ../../Classes/Skill.cpp \
                   ../../Classes/Soldier.cpp \
                   ../../Classes/WomanSoldier.cpp \
                   ../../Classes/World.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
