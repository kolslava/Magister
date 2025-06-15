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
    
} // namespace SharedLib
