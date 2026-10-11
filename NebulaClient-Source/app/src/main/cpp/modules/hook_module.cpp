#include "hook_module.hpp"
#include "../include/nebulaclient.hpp"

namespace Nebula {
    bool HookModule::InitializeGameHooks() {
        LOGE("[NOT IMPLEMENTED] InitializeGameHooks: Tiến trình game bị chặn hoặc chưa có offset.");
        return false;
    }

    void HookModule::RemoveHooks() {
        LOGI("HookModule: Đã gỡ bỏ hook an toàn.");
    }

    bool HookModule::IsInGameHookActive() {
        return false;
    }
}
