//
// Created by Nikita on 09.07.2021.
//

#ifndef EVOSCRIPT_TYPEDEFS_H
#define EVOSCRIPT_TYPEDEFS_H

#include <vector>
#include <string>
#include <any>

namespace EvoScript::Typedefs {
    /// сборник разных типов методов на все случаи жизни

    typedef void(*InitFnPtr)(void**);

    typedef void(*InitBehaviourFnPtr)();
    typedef void(*ReleaseBehaviourFnPtr)();
    typedef void(*SetPropertyFnPtr)(const std::string& id, const std::any& val);
    typedef std::any(*GetPropertyFnPtr)(const std::string& id);
    typedef std::vector<std::string>(*GetPropertiesFnPtr)();

    typedef void(*VoidFnPtr)();
    typedef void(*AwakeFnPtr)();
    typedef void(*StartFnPtr)();
    typedef void(*CloseFnPtr)();
    typedef void(*UpdateFnPtr)(float dt);
    typedef void(*FixedUpdateFnPtr)();
    typedef void(*OnGUIFnPtr)();
}

#endif //EVOSCRIPT_TYPEDEFS_H
