#pragma once
#include "../Libs/Libhat/libhat.hpp"
#include "../Libs/Libhat/libhat/Process.hpp"
#include "../Utils/MemoryUtils.h"

#define DEFINE(name, str) const hat::signature_view name = ([]() {  \
    static constexpr auto sig = hat::compile_signature<str>();      \
    return hat::signature_view{sig};                                \
})();

namespace Sigs {
    namespace Actor {
        extern const hat::signature_view IsInWaterHook;
        extern const hat::signature_view IsOnGroundHook;
        extern const hat::signature_view ActorNormalTickHook;
        
    }
    namespace Components {
        extern const  hat::signature_view getArmorComponent;
        extern const  hat::signature_view getActorHeadRotComponent;
    }
    namespace Hooks {
        extern const hat::signature_view KeyMapHook;
        extern const hat::signature_view KeyMouseHook;
        extern const hat::signature_view SetUpAndRenderHook;
        extern const hat::signature_view RenderLevelHook;
    }

    namespace tessellator {
        extern const hat::signature_view begin;
        extern const hat::signature_view vertex;
        extern const hat::signature_view color;
    }

    namespace Vtables {
        extern const hat::signature_view PlayerVtable;
    }
}