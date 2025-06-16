#pragma once

#include <string>

namespace SharedLib {

    struct AppConfig {
        // Налаштування для Сервера
        std::string server_host = "0.0.0.0";
        int server_port = 18081;
        std::string server_cert_path = "./cert/server.crt";
        std::string server_key_path = "./cert/server.key";

        // Налаштування для Клієнта
        std::string client_target_host = "localhost";
        bool client_verify_ssl = false; // Для самопідписаних сертифікатів

        // Метод для формування URL для клієнта
        std::string getClientTargetUrl(const std::string& endpoint) const {
            return "https://" + client_target_host + ":" + std::to_string(server_port) + endpoint;
        }
    };

} // namespace Shared