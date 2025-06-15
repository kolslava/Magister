/*

*/

#pragma once

#define JSONSERIALIZER_HPP
#ifdef JSONSERIALIZER_HPP

#include <string>
#include <optional>

//#include <nlohmann/json.hpp>

namespace SharedLib {
    
    class JsonSerializer {
    public:
        // --- Структури

        struct Test {
            std::string name;
        };

        // --- Методи для серіалізації/десеріалізації ---

        static std::string serialize(const Test& request);
        static std::optional<Test> deserializeTest(const std::string& jsonString);
    };

} // namespace SharedLib

#endif // JSONSERIALIZER_HPP