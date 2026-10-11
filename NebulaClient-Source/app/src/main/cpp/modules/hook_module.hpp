#ifndef HOOK_MODULE_HPP
#define HOOK_MODULE_HPP

namespace Nebula {
    class HookModule {
    public:
        static bool InitializeGameHooks();
        static void RemoveHooks();
        static bool IsInGameHookActive();
    };
}
#endif
