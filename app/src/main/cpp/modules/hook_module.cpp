#include "hook_module.hpp"
#include "../include/nebulaclient.hpp"
#include <atomic>

namespace Nebula {
    namespace {
        std::atomic<HookState> g_state{HookState::NotInitialized};
        std::atomic<std::size_t> g_installedHooks{0};
    }

    bool HookModule::InitializeGameHooks() {
        if (g_state.load() == HookState::Active) {
            LOGI("HookModule: hook đã active (%zu hook), bỏ qua khởi tạo lại.",
                 g_installedHooks.load());
            return true;
        }

        // Chưa có mục tiêu hook nào được xác minh (thiếu thư viện native và symbol),
        // nên không có gì để cài. Không đặt Active.
        g_state.store(HookState::NotImplemented);
        LOGE("[NOT IMPLEMENTED] InitializeGameHooks: chưa có mục tiêu hook được xác minh. "
             "Không cài hook nào.");
        return false;
    }

    void HookModule::RemoveHooks() {
        const std::size_t installed = g_installedHooks.load();
        if (installed != 0) {
            // Chưa có cơ chế gỡ vì chưa có cơ chế cài. Giữ nguyên trạng thái để không báo sai.
            LOGE("HookModule: ghi nhận %zu hook nhưng chưa có cơ chế gỡ; giữ nguyên trạng thái.",
                 installed);
            return;
        }
        g_state.store(HookState::NotInitialized);
        LOGI("HookModule: không có hook nào được cài, không cần gỡ.");
    }

    bool HookModule::IsInGameHookActive() {
        return g_state.load() == HookState::Active && g_installedHooks.load() > 0;
    }

    HookState HookModule::GetState() {
        return g_state.load();
    }

    std::size_t HookModule::InstalledHookCount() {
        return g_installedHooks.load();
    }

    const char* HookModule::StateName(HookState state) {
        switch (state) {
            case HookState::NotInitialized: return "NotInitialized";
            case HookState::NotImplemented: return "NotImplemented";
            case HookState::Failed:         return "Failed";
            case HookState::Active:         return "Active";
        }
        return "Unknown";
    }
}
