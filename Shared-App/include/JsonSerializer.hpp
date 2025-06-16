/*

*/

#pragma once

#define JSONSERIALIZER_HPP
#ifdef JSONSERIALIZER_HPP

#include <string>
#include <optional>

#include <map>

namespace SharedLib {
    
    class JsonSerializer {
    public:
        // --- Структури для реєстрації ---
        struct AgentRegisterRequest {
            std::string hostname;
            std::string os_version;
            std::string public_key_pem;
        };

        struct AgentRegisterResponse {
            long long agent_id;
        };

        // --- Структури для "Відбитка" ---
        struct FingerprintData {
            std::map<std::string, std::string> metrics;
            long long timestamp;
        };

        struct FingerprintSubmitRequest {
            FingerprintData data;
            std::string signature_base64;
        };

        struct FingerprintSubmitResponse {
            bool accepted;
            std::string message;
        };

        // --- Методи серіалізації/десеріалізації ---
        static std::string serialize(const AgentRegisterRequest& d);
        static std::optional<AgentRegisterRequest> deserializeRegisterRequest(const std::string& s);
        static std::string serialize(const AgentRegisterResponse& d);
        static std::optional<AgentRegisterResponse> deserializeRegisterResponse(const std::string& s);
        static std::string serialize(const FingerprintData& d);
        static std::string serialize(const FingerprintSubmitRequest& d);
        static std::optional<FingerprintSubmitRequest> deserializeFingerprintRequest(const std::string& s);
    };

} // namespace SharedLib

#endif // JSONSERIALIZER_HPP