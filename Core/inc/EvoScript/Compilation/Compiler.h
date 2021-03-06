//
// Created by Nikita on 03.07.2021.
//

#ifndef EVOSCRIPT_COMPILER_H
#define EVOSCRIPT_COMPILER_H

#include <EvoScript/Script.h>
#include <EvoScript/Tools/StringUtils.h>
#include <EvoScript/Tools/FileSystem.h>
#include <EvoScript/Tools/HashUtils.h>

namespace EvoScript {
    typedef std::unordered_map<std::string, std::vector<uint32_t>> ModuleCopies;

    class Compiler : public Tools::NonCopyable {
    public:
        explicit Compiler(std::string cachePath);
        ~Compiler() override = default;

    public:
        IState* AllocateState(const std::string& path);

        void SetApiVersion(std::string version);
        void SetGenerator(std::string generator);

        bool Compile(Script* script);

        bool TryLoad(Script* script);
        bool Load(Script* script);

        bool LoadState(IState* state);

    private:
        bool ClearModulesCache(const std::string& path);
        uint32_t FindFreeID(const std::string& pathToModule);
        bool CheckSourceHash(const std::string& source, const std::string& pathToScript, bool debug);
        bool CheckApiHash(const std::string& pathToScript, bool debug);

    private:
        std::string  m_apiVersion    = "None";
        std::string  m_generator     = "None";
        std::string  m_cachePath     = "None";

        std::recursive_mutex m_mutex = std::recursive_mutex();
        ModuleCopies m_moduleCopies  = ModuleCopies();

    };
}

#endif //EVOSCRIPT_COMPILER_H
