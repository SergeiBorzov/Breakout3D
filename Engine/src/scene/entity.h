#ifndef FLY_ENTITY_H
#define FLY_ENTITY_H

#include <string>

#include "transform.h"
#include "component.h"

namespace Fly {
    class Scene;

    class Entity {
    friend class Scene;
    public:
        template <typename T>
        T* GetComponent() {
            return (T*)m_Components[T::s_ComponentID - 1];
        }

        template <typename T>
        T* AddComponent() {
            T* component = new T(*this);
            // Ugly fix or feature?
            if (m_Components[T::s_ComponentID - 1]) {
                return (T*)m_Components[T::s_ComponentID - 1];
            }
            m_Components[T::s_ComponentID - 1] = (Component*)component;
            return component;
        }

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            T* component = new T(*this, std::forward<Args>(args)...);
            // Ugly fix or feature?
            if (m_Components[T::s_ComponentID - 1]) {
                return (T*)m_Components[T::s_ComponentID - 1];
            }
            m_Components[T::s_ComponentID - 1] = (Component*)component;
            return component;
        }

        template <typename T>
        void RemoveComponent() {
            if (m_Components[T::s_ComponentID - 1]) {
                delete m_Components[T::componentID - 1];
                m_Components[T::s_ComponentID - 1] = nullptr;
            }
        }

        void SetActive(bool value);
        bool IsActive() const { return active; }

        ~Entity();        
    private:
        Entity(Scene& s, const std::string& name);

        Component* m_Components[FLY_MAX_COMPONENTS] = {nullptr};
        bool active = true;
    public:
        Transform transform;
        Scene& scene;
        std::string name;
    };
}

#endif /* End of FLY_ENTITY_H */
