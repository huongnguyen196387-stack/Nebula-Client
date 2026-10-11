#include "renderer_module.hpp"
#include "../include/nebulaclient.hpp"

namespace Nebula {
    void RendererModule::InitInGameRenderer() {
        LOGI("[NOT IMPLEMENTED] InitInGameRenderer: Chờ tích hợp vòng lặp đồ họa thực tế của game.");
    }

    void RendererModule::ShutdownRenderer() {
        LOGI("RendererModule: chưa có tài nguyên renderer nào được tạo, không cần dọn dẹp.");
    }
}
