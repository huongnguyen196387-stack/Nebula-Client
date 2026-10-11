#ifndef NEBULACLIENT_HPP
#define NEBULACLIENT_HPP

#include <android/log.h>
#include <string>
#include <exception>

#define LOG_TAG "NebulaNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace Nebula {
    bool InitModules();
}

#endif
