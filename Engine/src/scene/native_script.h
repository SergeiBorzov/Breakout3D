#ifndef FLY_NATIVE_SCRIPT_H
#define FLY_NATIVE_SCRIPT_H

#include "entity.h"

namespace Fly {
    class Collider;

    class NativeScript: public Component {
    public:
        NativeScript(Fly::Entity& entity);
        
        virtual void OnStart() {}
        virtual void OnUpdate(f32 deltaTime) {}
        virtual void OnFixedUpdate(f32 deltaTime) {}

        virtual void OnCollisionEnter(Collider& other) {}
        virtual void OnCollision(Collider& other) {}
        virtual void OnCollisionExit(Collider& other) {}

        virtual void OnTriggerEnter(Collider& other) {}
        virtual void OnTrigger(Collider& other) {}
        virtual void OnTriggerExit(Collider& other) {}

        virtual ~NativeScript();

        FLY_API static const u32 s_ComponentID;
    };
    
}

#endif /* End of FLY_NATIVE_SCRIPT_H */
