#pragma once

#include <scene/GameComponent.h>
#include <scene/ScriptableObject.h>

namespace Engine
{

class NativeScript : public GameComponent
{
public:
    ScriptableObject* instance = nullptr;

    ScriptableObject*(*instantiateScript)();
    void (*destroyScript)(NativeScript*);

    template<typename T, typename... Args>
    void bind(Args&&... args)
    {
        instantiateScript = [&args...]()
        {
            return static_cast<ScriptableObject*>(new T(std::forward<Args>(args)...));
        };

        destroyScript = [](NativeScript* component)
        {
            delete component->instance;
            component->instance = nullptr;
        };
    }
};

}