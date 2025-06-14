#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace Shared {

    // Проста структура для передачі даних
    struct Message {
        std::string text;
        std::string author;
    };

    // Функції для серіалізації/десеріалізації в JSON
    void to_json(nlohmann::json& j, const Message& m);
    void from_json(const nlohmann::json& j, Message& m);

} // namespace Shared