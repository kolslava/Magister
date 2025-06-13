#include <iostream>
#include "cpr/cpr.h"
#include "spdlog/spdlog.h"
#include "shared.hpp"

int main() {
    std::string url = "http://localhost:18080/api/message";
    spdlog::info("Sending request to {}", url);

    // Робимо GET запит
    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code == 200) {
        spdlog::info("Server response OK (200)");
        try {
            // Парсимо JSON з тіла відповіді
            nlohmann::json response_json = nlohmann::json::parse(r.text);
            // Десеріалізуємо JSON у нашу структуру
            Shared::Message msg = response_json.get<Shared::Message>();

            spdlog::info("Message received successfully!");
            fmt::print("----------------------------------------\n");
            fmt::print("Message: {}\n", msg.text);
            fmt::print("Author:  {}\n", msg.author);
            fmt::print("----------------------------------------\n");

        } catch (const nlohmann::json::parse_error& e) {
            spdlog::error("Failed to parse JSON: {}", e.what());
        }
    } else {
        spdlog::error("Request failed. Status code: {}, Error: {}", r.status_code, r.error.message);
    }

    return 0;
}