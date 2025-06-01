#pragma once
#include "EntityContext.h"
#include "../../Utils/MemoryUtils.h"
#include "../Components/StateVectorComponent.h"
#include "Level.h/Level.h"
#include "../Components/ActorRotationComponent.h"
#include "../Components/ActorEquipmentComponent.h"
#include "../Components/ActorHeadRotationComponent.h"
class Actor {
public:
    //这些是通过“loading actor找到的，对比新旧版本”
    BUILD_ACCESS(this, Level*, level, 0x1D8)//1.21.82
    BUILD_ACCESS(this, StateVectorComponent*, stateVector, 0x218)//1.21.82
        //ActorRotationComponent
    BUILD_ACCESS(this, ActorRotationComponent*, rotation, 0x228)//1.21.82


    Vec3<float> getPos() {
        return stateVector->pos;
    }
    bool isInWater() {
        using func_t = bool(__thiscall*)(Actor*);
        static func_t func = (func_t)findSig(Sigs::Actor::IsInWaterHook);
        return func(this);
    }

    bool isOnGround() {
        using func_t = bool(__thiscall*)(Actor*);
        static func_t func = (func_t)findSig(Sigs::Actor::IsOnGroundHook);
        return func(this);
    }

    EntityContext* getEntityContext()
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(this);
        return reinterpret_cast<EntityContext*>((uintptr_t)this + 0x8); // 1.21.82
    }

    ActorEquipmentComponent* getActorEquipmentComponent() {
        using func_t = ActorEquipmentComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)findCall(findSig(Sigs::Components::getArmorComponent));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    ActorHeadRotationComponent* getActorHeadRotation() {
        using func_t = ActorHeadRotationComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)findCall(findSig(Sigs::Components::getActorHeadRotComponent));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    template <typename T>
    T* getComponent() {
        return const_cast<T*>(getEntityContext()->getRegistry().try_get<T>(getEntityContext()->mEntity));
    }

    template <typename T>
    bool hasComponent() {
        return getEntityContext()->getRegistry().all_of<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void getOrEmplaceComponent() {
        return getEntityContext()->getRegistry().get_or_emplace<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void removeComponent() {
        getEntityContext()->getRegistry().remove<T>(getEntityContext()->mEntity);
    }

};