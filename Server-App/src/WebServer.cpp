#include <WebServer.hpp>

// Підключаємо всі необхідні залежності ТІЛЬКИ ТУТ

#include <crow.h>
//#include <DatabaseManager.hpp>
#include <JsonSerializer.hpp>
//#include <Config.hpp>
#include <Logger.hpp>

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

        
        CROW_ROUTE(m_app, "/health")([](){
            SharedLib::Logger::info("GET /health request received.");

            // 1. Створюємо нашу власну, просту структуру
            SharedLib::JsonSerializer::HealthStatus status;
            status.status = "ok";
            status.service = "DeviceProfiler Server";

            // 2. Викликаємо наш серіалізатор, щоб отримати рядок JSON
            std::string json_response = SharedLib::JsonSerializer::serialize(status);

            // 3. Відправляємо відповідь
            crow::response res(200, json_response);
            res.add_header("Content-Type", "application/json");
            return res;
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