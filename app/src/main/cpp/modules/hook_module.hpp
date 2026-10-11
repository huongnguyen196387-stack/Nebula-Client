#ifndef HOOK_MODULE_HPP
#define HOOK_MODULE_HPP

#include <cstddef>

namespace Nebula {
    // Active và Failed chỉ được đặt bởi mã cài hook thật. Hiện chưa có mã
    // cài hook nào, nên hai trạng thái này chưa thể đạt tới.
    enum class HookState {
        NotInitialized,
        NotImplemented,
        Failed,
        Active
    };

    class HookModule {
    public:
        // Trả về true chỉ khi có ít nhất một hook đã được cài thật.
        static bool InitializeGameHooks();
        static void RemoveHooks();
        static bool IsInGameHookActive();
        static HookState GetState();
        static std::size_t InstalledHookCount();
        static const char* StateName(HookState state);
    };
}
#endif
