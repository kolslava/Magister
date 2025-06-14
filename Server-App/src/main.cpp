// /Server-App/src/main.cpp - ВИПРАВЛЕНА ВЕРСІЯ

#include "crow.h"
#include "spdlog/spdlog.h"
#include "shared.hpp"

int main() {
    crow::SimpleApp app;

    // Створюємо "ручку" API
    CROW_ROUTE(app, "/api/message")([](){
        spdlog::info("GET /api/message request received");

        // Готуємо дані для відправки
        Shared::Message msg;
        msg.text = "Вітання з мінімального сервера!";
        msg.author = "Crow & C++";

        // Створюємо JSON-об'єкт
        nlohmann::json response_json = msg;
        
        // Створюємо відповідь, передаючи текстове представлення JSON (.dump())
        crow::response res(200, response_json.dump());
        
        // Встановлюємо заголовок, щоб клієнт знав, що це JSON
        res.add_header("Content-Type", "application/json");
        
        return res;
    });

    spdlog::info("Server starting on port 18080...");
    app.port(18080).run();

    return 0;
}