LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/Animator.cpp \
				   ../../Classes/AppDelegate.cpp \
				   ../../Classes/ComponentObject.cpp \
				   ../../Classes/ChapterLayer.cpp \
				   ../../Classes/DataController.cpp \
				   ../../Classes/EffectComponent.cpp \
				   ../../Classes/EffectItem.cpp \
				   ../../Classes/Enemy.cpp \
				   ../../Classes/EntityManager.cpp \
				   ../../Classes/GameObject.cpp \
				   ../../Classes/HubLayer.cpp \
				   ../../Classes/Item.cpp \
				   ../../Classes/LevelsLayer.cpp \
				   ../../Classes/LoadingLayer.cpp \
				   ../../Classes/MenuLayer.cpp \
				   ../../Classes/Player.cpp \
				   ../../Classes/PlayLayer.cpp \
				   ../../Classes/ResultLayer.cpp \
				   ../../Classes/SplashLayer.cpp \
				   ../../Classes/AboutLayer.cpp \
				   ../../Classes/Utils.cpp \
				   ../../Classes/AdmobHelper.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

$(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
