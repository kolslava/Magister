/*

*/

#pragma once 

#define LOGGER_HPP
#ifdef LOGGER_HPP

#include <string>
#include <memory>

#include <spdlog/spdlog.h>

namespace SharedLib {
    
    /**
     * @brief Статичний клас-фасад для глобального логера.
     * Інкапсулює всю взаємодію з бібліотекою spdlog.
     */
    class Logger {
    public:
        static void Init(const std::string& loggerName, const std::string& logFilePath);

        // --- Обгортки для функцій логування ---

        template <typename... Args>
        static void trace(fmt::format_string<Args...> fmt, Args&&... args) {
            spdlog::trace(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void info(fmt::format_string<Args...> fmt, Args&&... args) {
            spdlog::info(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void warn(fmt::format_string<Args...> fmt, Args&&... args) {
            spdlog::warn(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void error(fmt::format_string<Args...> fmt, Args&&... args) {
            spdlog::error(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void critical(fmt::format_string<Args...> fmt, Args&&... args) {
            spdlog::critical(fmt, std::forward<Args>(args)...);
        }
    };

} // namespace SharedLib


#endif // LOGGER_HPP