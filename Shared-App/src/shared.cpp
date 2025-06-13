#include "shared.hpp"

namespace Shared {

    void to_json(nlohmann::json& j, const Message& m) {
        j = nlohmann::json{
            {"text", m.text},
            {"author", m.author}
        };
    }

    void from_json(const nlohmann::json& j, Message& m) {
        j.at("text").get_to(m.text);
        j.at("author").get_to(m.author);
    }
    
} // namespace Shared