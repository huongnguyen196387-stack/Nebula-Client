#include "feature_module.hpp"

namespace Nebula {
    namespace FeatureModule {
        static bool fullbrightState = false;

        void SetFullbright(bool enabled) {
            fullbrightState = enabled;
            // [NOT IMPLEMENTED] Yêu cầu can thiệp shader ánh sáng của game.
        }

        bool GetFullbright() {
            return fullbrightState;
        }
    }
}
