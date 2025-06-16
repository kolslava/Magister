#include <WebServer.hpp>

// Підключаємо всі необхідні залежності ТІЛЬКИ ТУТ

#include <crow.h>
#include <DatabaseManager.hpp>

#include <Shared.hpp>

/**
 * @brief Внутрішній клас реалізації (патерн PIMPL).
 */
class WebServer::Impl {
public:
    // Конструктор Impl отримує налаштування і готує сервер
    Impl(uint16_t port, const std::string& cert_path, const std::string& key_path)
        : m_app(), 
          m_port(port), 
          m_cert_path(cert_path), 
          m_key_path(key_path) {
        
        SharedLib::Logger::info("Setting up basic server routes...");
        setupRoutes();
    }

    // Метод для запуску сервера
    void run() {
        SharedLib::Logger::info("WebServer starting on port {} (HTTPS)...", m_port);
        m_app.port(m_port)
           .ssl_file(m_cert_path, m_key_path) // Використовуємо передані шляхи
           .multithreaded()
           .run();
    }

private:
    // Тут ми визначаємо всі наші API "ручки"
    void setupRoutes() {
        // Найпростіший маршрут
        CROW_ROUTE(m_app, "/")([](){
            SharedLib::Logger::info("GET / request received.");
            return "Hello, this is a basic HTTPS server!";
        });

        // Ендпоінт для реєстрації нового агента
        CROW_ROUTE(m_app, "/api/v1/agents").methods("POST"_method)
        // ВИПРАВЛЕНО: Лямбда більше не захоплює [&db], бо він їй не потрібен
        ([](const crow::request& req){
            SharedLib::Logger::info("Received new agent registration request from {}", req.remote_ip_address);

            auto regRequestOpt = SharedLib::JsonSerializer::deserializeRegisterRequest(req.body);
            if (!regRequestOpt) {
                SharedLib::Logger::error("Bad registration request from {}: invalid JSON.", req.remote_ip_address);
                return crow::response(400, "Bad Request: Invalid JSON");
            }
            auto& regRequest = *regRequestOpt;

            Agent agent_to_db; // Припускаємо, що struct Agent визначено у вашому DatabaseManager.hpp
            agent_to_db.hostname = regRequest.hostname;
            agent_to_db.os_version = regRequest.os_version;
            agent_to_db.public_key = regRequest.public_key_pem;
            
            try {
                // ВИПРАВЛЕНО: Отримуємо доступ до БД через статичний метод get()
                auto& db = DatabaseManager::get();
                int new_id = db.addAgent(agent_to_db);

                SharedLib::Logger::info("Successfully registered new agent '{}' from {} with ID: {}", 
                                     agent_to_db.hostname, req.remote_ip_address, new_id);

                SharedLib::JsonSerializer::AgentRegisterResponse response{ (long long)new_id };
                crow::response res(200, SharedLib::JsonSerializer::serialize(response));
                res.add_header("Content-Type", "application/json");
                return res;

            } catch (const std::exception& e) {
                SharedLib::Logger::error("Failed to insert agent from {} into database: {}", req.remote_ip_address, e.what());
                return crow::response(500, "Internal Server Error");
            }
        });

        // НОВИЙ ЕНДПОІНТ для прийому відбитків
        CROW_ROUTE(m_app, "/api/v1/agents/<int>/fingerprints").methods("POST"_method)
        ([](const crow::request& req, int agent_id){
            SharedLib::Logger::info("Received fingerprint from agent ID: {}", agent_id);

            auto& db = DatabaseManager::get();

            // 1. Дістаємо публічний ключ агента з БД
            auto pubKeyOpt = db.getAgentPublicKey(agent_id);
            if (!pubKeyOpt) {
                return crow::response(404, "Agent not found");
            }

            // 2. Десеріалізуємо запит
            auto requestOpt = SharedLib::JsonSerializer::deserializeFingerprintRequest(req.body);
            if (!requestOpt) {
                return crow::response(400, "Bad Request: Invalid JSON");
            }
            auto& request = *requestOpt;

            // 3. Готуємо дані для перевірки підпису (це серіалізована частина 'data')
            std::string data_to_verify = SharedLib::JsonSerializer::serialize(request.data);

            // 4. Перевіряємо підпис
            
            bool is_valid = SharedLib::SignatureManager::verify(*pubKeyOpt, data_to_verify, request.signature_base64);

            if (is_valid) {
                SharedLib::Logger::info("Signature for agent {} is VALID.", agent_id);
                
                // Зберігаємо відбиток у БД
                Fingerprint fp_to_db;
                fp_to_db.agent_id = agent_id;
                fp_to_db.data = SharedLib::JsonSerializer::serialize(request.data); // Зберігаємо як JSON
                db.addFingerprint(fp_to_db);

                SharedLib::Logger::info("Fingerprint data saved to DB.");
                return crow::response(200, "Fingerprint accepted");
            } else {
                SharedLib::Logger::warn("Signature for agent {} is INVALID!", agent_id);
                return crow::response(401, "Unauthorized: Invalid signature");
            }
        });

    }

    crow::SimpleApp m_app;
    uint16_t m_port;
    std::string m_cert_path;
    std::string m_key_path;
};


// --- Реалізація методів WebServer ---

WebServer::WebServer(uint16_t port, const std::string& cert_path, const std::string& key_path) 
    : pimpl(std::make_unique<Impl>(port, cert_path, key_path)) {}

WebServer::~WebServer() = default; 

void WebServer::run() {
    pimpl->run();
}