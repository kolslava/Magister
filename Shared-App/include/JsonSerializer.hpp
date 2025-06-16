/*

*/

#pragma once

#define JSONSERIALIZER_HPP
#ifdef JSONSERIALIZER_HPP

#include <string>
#include <optional>

#include <map>

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

        // --- Структури для "Відбитка" ---
        struct FingerprintData {
            // "Відбиток" - це набір пар "метрика: значення"
            std::map<std::string, std::string> metrics;
            long long timestamp;
        };

        struct FingerprintSubmitRequest {
            // Самі дані "відбитка"
            FingerprintData data;
            // Цифровий підпис цих даних (у форматі Base64)
            std::string signature_base64;
        };

        // --- Структури для реєстрації ---
        struct AgentRegisterRequest {
            std::string hostname;
            std::string os_version;
            std::string public_key_pem;
        };

        struct AgentRegisterResponse {
            long long agent_id;
        };

        // --- Методи для серіалізації/десеріалізації ---

        static std::string serialize(const Test& request);
        static std::optional<Test> deserializeTest(const std::string& jsonString);

        // НОВІ МЕТОДИ для HealthStatus
        static std::string serialize(const HealthStatus& data);
        // Десеріалізатор нам поки не потрібен, але можемо додати для повноти
        static std::optional<HealthStatus> deserializeHealthStatus(const std::string& jsonString);

        // Для відбитка
        static std::string serialize(const FingerprintData& data);
        static std::string serialize(const FingerprintSubmitRequest& request);
        static std::optional<FingerprintSubmitRequest> deserializeFingerprintRequest(const std::string& jsonString);
    
        // Для реєстрації
        static std::string serialize(const AgentRegisterRequest& request);
        static std::optional<AgentRegisterRequest> deserializeRegisterRequest(const std::string& jsonString);

        static std::string serialize(const AgentRegisterResponse& response);
        static std::optional<AgentRegisterResponse> deserializeRegisterResponse(const std::string& jsonString);
    };

} // namespace SharedLib

#endif // JSONSERIALIZER_HPP