#pragma once

#include "Core/Interface/ILightInterface.h"

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

/*
 * Registered Objects Class Name to be created Dynamically
 * Must be registered manually for showing up on the list.
 */
class RegistryLight
{
public:

    using CreateFunc = std::function<std::unique_ptr<ILightInterface>()>;

    static void Register(const std::string& name, CreateFunc createFunc)
    {
        if (registry_.contains(name)) return;

        registry_[name] = std::move(createFunc);
        mNames.push_back(name);
    }
    static std::unique_ptr<ILightInterface> Create(const std::string& name)
    {
        auto it = registry_.find(name);
        return it != registry_.end() ? it->second() : nullptr;
    }
    static std::vector<std::string>& GetRegisteredNames()
    {
        return mNames;
    }

private:
    inline static std::unordered_map<std::string, CreateFunc> registry_;
    inline static std::vector<std::string> mNames;
};

#define REGISTER_LIGHT(CLASS_NAME) \
    namespace { \
        struct CLASS_NAME##Registrar { \
            CLASS_NAME##Registrar() { \
                RegistryLight::Register(#CLASS_NAME, []() { return std::make_unique<CLASS_NAME>(); }); \
            } \
        }; \
        static CLASS_NAME##Registrar CLASS_NAME##_registrar; \
    }
