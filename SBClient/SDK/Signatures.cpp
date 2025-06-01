#include "Signatures.h"

//gamedata
DEFINE(Sigs::Hooks::KeyMapHook, "48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89")
DEFINE(Sigs::Hooks::KeyMouseHook, "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9")
DEFINE(Sigs::Hooks::SetUpAndRenderHook, "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA")
DEFINE(Sigs::Hooks::RenderLevelHook, "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 44 24 ? 4C 8B EA 4C 8B F9")

//tessellator
DEFINE(Sigs::tessellator::begin, "48 89 5C 24 ? 56 48 83 EC ? 80 B9 ? ? ? ? ? 45 0F B6")
DEFINE(Sigs::tessellator::vertex, "40 57 48 81 EC ? ? ? ? 0F 29 7C ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 81")
DEFINE(Sigs::tessellator::color, "80 b9 ? ? ? ? ? 4c 8b c1 75")

//Actor
//
DEFINE(Sigs::Actor::IsInWaterHook,"48 83 EC ? 4C 8B D9 48 8D 54 24 ? 48 8B 49 ? 8B 41 ? 48 8B 49 ? 89 44 24 ? E8 ? ? ? ? 48 85 C0 74 ? ? ? 48 0F BA E0 ? 73 ? 41 80 7B")//this is is in water or rain
DEFINE(Sigs::Actor::IsOnGroundHook,"4C 8B ?? ?? 41 ?? ?? ?? ?? ?? 8B 49 ?? 49 8B ?? ?? 49 8B ?? ?? 4D 8B ?? ?? 48 2B C2 48 ?? ?? ?? 48 FF C8 49 23 C2 48 8B 04 C2")
DEFINE(Sigs::Actor::ActorNormalTickHook, "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F9 45 33 ED")

//component
DEFINE(Sigs::Components::getArmorComponent,"E8 ? ? ? ? 48 85 C0 74 ? 48 8B 48 ? ? ? ? F3 0F 2C D0")
DEFINE(Sigs::Components::getActorHeadRotComponent,"E8 ? ? ? ? 48 85 C0 74 ? ? ? ? ? F3 0F 11 40 ? 33 D2")

//vtable
//? ? ? BA ? ? ? ? 44 8D 4A ? 44 8D 42 ? 66 C7 44 24 ? ? ? E8 ? ? ? ? 48 8B 8F
DEFINE(Sigs::Vtables::PlayerVtable,"? ? ? BA ? ? ? ? 44 8D 4A ? 44 8D 42 ? 66 C7 44 24 ? ? ? E8 ? ? ? ? 48 8B 8F")