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

        // НОВА СТРУКТУРА для відповіді /health
        struct HealthStatus {
            std::string status;
            std::string service;
        };

        // --- Методи для серіалізації/десеріалізації ---

        static std::string serialize(const Test& request);
        static std::optional<Test> deserializeTest(const std::string& jsonString);

        // НОВІ МЕТОДИ для HealthStatus
        static std::string serialize(const HealthStatus& data);
        // Десеріалізатор нам поки не потрібен, але можемо додати для повноти
        static std::optional<HealthStatus> deserializeHealthStatus(const std::string& jsonString);
    };

} // namespace SharedLib

#endif // JSONSERIALIZER_HPP