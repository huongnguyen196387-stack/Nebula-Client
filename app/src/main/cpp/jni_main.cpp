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
        const bool hooksReady = HookModule::InitializeGameHooks();
        LOGI("Nebula::InitModules: hookState=%s, hooksReady=%d, installedHooks=%zu",
             HookModule::StateName(HookModule::GetState()), hooksReady ? 1 : 0,
             HookModule::InstalledHookCount());
        // Hook là tùy chọn: scaffold vẫn khởi tạo được khi hook không khả dụng.
        return true;
    }

    void ShutdownModules() {
        HookModule::RemoveHooks();
        RendererModule::ShutdownRenderer();
    }
}

namespace {
    std::string BuildStatusString() {
        using Nebula::HookModule;
        using Nebula::HookState;
        switch (HookModule::GetState()) {
            case HookState::NotInitialized:
                return "Standby (Native Loaded - hooks chưa khởi tạo)";
            case HookState::NotImplemented:
                return "Standby (Native Loaded - Game Hook NOT IMPLEMENTED)";
            case HookState::Failed:
                return "Hook FAILED (đã tắt an toàn)";
            case HookState::Active:
                return HookModule::IsInGameHookActive()
                           ? "Active (In-Game Hooked)"
                           : "Inconsistent (Active nhưng không có hook nào được cài)";
        }
        return "Unknown";
    }
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_nebula_MainActivity_nativeInitClient(JNIEnv *, jobject) {
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
Java_com_nebula_MainActivity_nativeGetStatus(JNIEnv *env, jobject) {
    try {
        std::string status = BuildStatusString();
        return env->NewStringUTF(status.c_str());
    } catch (...) {
        LOGE("Lỗi ngoại lệ trong nativeGetStatus");
        return env->NewStringUTF("Status unavailable (native exception)");
    }
}

JNIEXPORT jboolean JNICALL
Java_com_nebula_MainActivity_nativeRunOpenGlesTest(JNIEnv *, jobject) {
    try {
        return Nebula::OpenGlesTestHarness::RunTestContext() ? JNI_TRUE : JNI_FALSE;
    } catch (...) {
        return JNI_FALSE;
    }
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *, void *) {
    try {
        Nebula::ShutdownModules();
    } catch (...) {
    }
}

}
