LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

V_SRC_PATH     := <fill me in>
V_SRC_LIB_PATH := <fill me in>
V_GJ_PATH      := ../../../../source
V_GJ_LIB_PATH  := ../../../../libraries

##########################################################################

## these lines copy the shared library to the libs folder
## <LOCAL_SHARED_LIBRARIES := libcurl> tells the linker about it
## don't forget to load the library <curl> in your Java file

LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := $(V_GJ_LIB_PATH)/bin/android/$(TARGET_ARCH_ABI)/libcurl.so
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

##########################################################################

LOCAL_MODULE := Main

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(V_GJ_LIB_PATH)/include  \
                    $(LOCAL_PATH)/$(V_GJ_PATH)              \
                    $(LOCAL_PATH)/$(V_SRC_PATH) 

LOCAL_SRC_FILES := $(V_GJ_PATH)/Base64.cpp                               \
                   $(V_GJ_PATH)/MD5.cpp                                  \
                   $(V_GJ_PATH)/gjAPI.cpp                                \
                   $(V_GJ_PATH)/gjDataItem.cpp                           \
                   $(V_GJ_PATH)/gjNetwork.cpp                            \
                   $(V_GJ_PATH)/gjScore.cpp                              \
                   $(V_GJ_PATH)/gjTrophy.cpp                             \
                   $(V_GJ_PATH)/gjUser.cpp

LOCAL_SHARED_LIBRARIES := libcurl
LOCAL_LDLIBS           := -ldl -llog -landroid
LOCAL_CPPFLAGS         := -std=c++11

include $(BUILD_SHARED_LIBRARY)
