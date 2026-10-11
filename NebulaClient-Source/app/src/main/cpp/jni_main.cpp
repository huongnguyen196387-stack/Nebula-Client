#include <jni.h>
#include <string>
#include "include/nebulaclient.hpp"
#include "modules/hook_module.hpp"
#include "modules/renderer_module.hpp"
#include "modules/feature_module.hpp"
#include "modules/opengl_test_harness.hpp"

namespace Nebula {
    bool InitModules() {
        LOGI("Nebula::InitModules: Native scaffold đã được nạp thành công.");
        LOGI("TRẠNG THÁI: Hook, in-game renderer và các tính năng trong game CHƯA ĐƯỢC TRIỂN KHAI (NOT IMPLEMENTED).");
        return true;
    }
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_nebula_MainActivity_nativeInitClient(JNIEnv *env, jobject thiz) {
    try {
        bool initSuccess = Nebula::InitModules();
        if (!initSuccess) {
            LOGE("NebulaClient: Khởi tạo module bắt buộc thất bại!");
            return JNI_FALSE;
        }
        return JNI_TRUE;
    } catch (...) {
        LOGE("Lỗi ngoại lệ trong nativeInitClient");
        return JNI_FALSE;
    }
}

JNIEXPORT jstring JNICALL
Java_com_nebula_MainActivity_nativeGetStatus(JNIEnv *env, jobject thiz) {
    bool hooked = Nebula::HookModule::IsInGameHookActive();
    std::string status = hooked ? "Active (In-Game Hooked)" : "Standby (Native Loaded - Game Hook NOT IMPLEMENTED)";
    return env->NewStringUTF(status.c_str());
}

JNIEXPORT jboolean JNICALL
Java_com_nebula_MainActivity_nativeRunOpenGlesTest(JNIEnv *env, jobject thiz) {
    try {
        return Nebula::OpenGlesTestHarness::RunTestContext() ? JNI_TRUE : JNI_FALSE;
    } catch (...) {
        return JNI_FALSE;
    }
}

}
