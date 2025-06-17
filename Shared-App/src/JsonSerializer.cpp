#include <JsonSerializer.hpp>

#include "Logger.hpp"
#include <nlohmann/json.hpp>

namespace nlohmann {
    // Для FingerprintData
    void to_json(json& j, const SharedLib::JsonSerializer::FingerprintData& data) {
        j = json{
            {"metrics", data.metrics},
            {"timestamp", data.timestamp}
        };
    }
    void from_json(const json& j, SharedLib::JsonSerializer::FingerprintData& data) {
        j.at("metrics").get_to(data.metrics);
        j.at("timestamp").get_to(data.timestamp);
    }

    // Для FingerprintSubmitRequest
    void to_json(json& j, const SharedLib::JsonSerializer::FingerprintSubmitRequest& req) {
        j = json{
            {"data", req.data},
            {"signature", req.signature_base64}
        };
    }

    void from_json(const json& j, SharedLib::JsonSerializer::FingerprintSubmitRequest& req) {
        j.at("data").get_to(req.data);
        j.at("signature").get_to(req.signature_base64);
    }

    // Для AgentRegisterRequest
    void to_json(json& j, const SharedLib::JsonSerializer::AgentRegisterRequest& req) {
        j = json{
            {"hostname", req.hostname},
            {"os_version", req.os_version},
            {"public_key", req.public_key_pem}
        };
    }
    void from_json(const json& j, SharedLib::JsonSerializer::AgentRegisterRequest& req) {
        j.at("hostname").get_to(req.hostname);
        j.at("os_version").get_to(req.os_version);
        j.at("public_key").get_to(req.public_key_pem);
    }

    // Для AgentRegisterResponse
    void to_json(json& j, const SharedLib::JsonSerializer::AgentRegisterResponse& res) {
        j = json{{"agent_id", res.agent_id}};
    }
    void from_json(const json& j, SharedLib::JsonSerializer::AgentRegisterResponse& res) {
        j.at("agent_id").get_to(res.agent_id);
    }

    void to_json(json& j, const SharedLib::JsonSerializer::EnrollmentTokenResponse& res) {
        j = json{{"token", res.token}};
    }

    // Для AgentConfig
    void to_json(json& j, const SharedLib::JsonSerializer::AgentConfig& cfg) {
        j = json{};
        if (cfg.agent_id) {
            j["agent_id"] = *cfg.agent_id;
        }
        if (cfg.enrollment_token) {
            j["enrollment_token"] = *cfg.enrollment_token;
        }
    }
    void from_json(const json& j, SharedLib::JsonSerializer::AgentConfig& cfg) {
        if (j.contains("agent_id")) {
            cfg.agent_id = j.at("agent_id").get<long long>();
        }
        if (j.contains("enrollment_token")) {
            cfg.enrollment_token = j.at("enrollment_token").get<std::string>();
        }
    }
}

namespace SharedLib {
    // Для відбитка
    std::string JsonSerializer::serialize(const FingerprintData& data) { return nlohmann::json(data).dump(); }
    std::string JsonSerializer::serialize(const FingerprintSubmitRequest& request) { return nlohmann::json(request).dump(4); }

    std::optional<JsonSerializer::FingerprintSubmitRequest> JsonSerializer::deserializeFingerprintRequest(const std::string& jsonString) {
        try { return nlohmann::json::parse(jsonString).get<FingerprintSubmitRequest>(); }
        catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for FingerprintSubmitRequest: {}", e.what());
            return std::nullopt;
        }
    }

    // Для реєстрації
    std::string JsonSerializer::serialize(const AgentRegisterRequest& request) { return nlohmann::json(request).dump(4); }
    std::optional<JsonSerializer::AgentRegisterRequest> JsonSerializer::deserializeRegisterRequest(const std::string& jsonString) {
        try { return nlohmann::json::parse(jsonString).get<AgentRegisterRequest>(); }
        catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for AgentRegisterRequest: {}", e.what());
            return std::nullopt;
        }
    }
    std::string JsonSerializer::serialize(const AgentRegisterResponse& response) { return nlohmann::json(response).dump(4); }
    std::optional<JsonSerializer::AgentRegisterResponse> JsonSerializer::deserializeRegisterResponse(const std::string& jsonString) {
        try { return nlohmann::json::parse(jsonString).get<AgentRegisterResponse>(); }
        catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for AgentRegisterResponse: {}", e.what());
            return std::nullopt;
        }
    }

    // Новий метод для відповіді з токеном
    std::string JsonSerializer::serialize(const EnrollmentTokenResponse& data) {
        return nlohmann::json(data).dump(4);
    }

    std::optional<JsonSerializer::AgentConfig> JsonSerializer::deserializeAgentConfig(const std::string& jsonString) {
        try {
            if (jsonString.empty()) {
                return AgentConfig{}; // Повертаємо порожній конфіг, якщо файл порожній або відсутній
            }
            return nlohmann::json::parse(jsonString).get<AgentConfig>();
        } catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for AgentConfig: {}", e.what());
            return std::nullopt;
        }
    }

    std::string JsonSerializer::serialize(const AgentConfig& config) {
        return nlohmann::json(config).dump(4);
    }

} // namespace SharedLib
