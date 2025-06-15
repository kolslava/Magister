#include <Logger.hpp>

// Підключаємо потрібні заголовки spdlog
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <vector>

namespace SharedLib {

void Logger::Init(const std::string& loggerName, const std::string& logFilePath) {
    std::vector<spdlog::sink_ptr> sinks;

    // 1. Приймач для консолі
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$");
    sinks.push_back(console_sink);

    // 2. Приймач для файлу з ротацією
    auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFilePath, 1024 * 1024 * 5, 3);
    rotating_file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");
    sinks.push_back(rotating_file_sink);

    // 3. Створюємо логер
    auto logger = std::make_shared<spdlog::logger>(loggerName, begin(sinks), end(sinks));
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);

    // 4. Реєструємо як глобальний
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    
    // Використовуємо вже нашу власну обгортку для першого повідомлення!
    SharedLib::Logger::info("Logger initialized successfully for '{}'.", loggerName);
}

} // namespace SharedLib