#include <JsonSerializer.hpp>

#include "Logger.hpp"
#include <nlohmann/json.hpp>

namespace nlohmann {
    void to_json(json& j, const SharedLib::JsonSerializer::Test& req) {
        j = json{
            {"name", req.name},
        };
    }
    void from_json(const json& j, SharedLib::JsonSerializer::Test& req) {
        j.at("name").get_to(req.name);
    }

    // НОВА реалізація для HealthStatus
    void to_json(json& j, const SharedLib::JsonSerializer::HealthStatus& data) {
        j = json{
            {"status", data.status},
            {"service", data.service}
        };
    }

    void from_json(const json& j, SharedLib::JsonSerializer::HealthStatus& data) {
        j.at("status").get_to(data.status);
        j.at("service").get_to(data.service);
    }

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
}

namespace SharedLib {

    std::string JsonSerializer::serialize(const Test& request) {
        return nlohmann::json(request).dump(4);
    }

    std::optional<JsonSerializer::Test> JsonSerializer::deserializeTest(const std::string& jsonString) {
        try { return nlohmann::json::parse(jsonString).get<Test>(); }
        catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for Test: {}", e.what());
            return std::nullopt;
        }
    }

    // --- НОВІ методи для HealthStatus ---
    std::string JsonSerializer::serialize(const HealthStatus& data) {
        return nlohmann::json(data).dump(4);
    }

    std::optional<JsonSerializer::HealthStatus> JsonSerializer::deserializeHealthStatus(const std::string& jsonString) {
        try { return nlohmann::json::parse(jsonString).get<HealthStatus>(); }
        catch (const nlohmann::json::exception& e) {
            Logger::error("JSON deserialization failed for HealthStatus: {}", e.what());
            return std::nullopt;
        }
    }
    
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

} // namespace SharedLib
