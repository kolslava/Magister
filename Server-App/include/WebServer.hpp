/*

*/

#pragma once

#define WEBSERVER_HPP
#ifdef WEBSERVER_HPP

#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Клас, що інкапсулює всю логіку веб-сервера.
 */

class WebServer {
public:
    /**
     * @brief Конструктор, що приймає налаштування для запуску.
     * @param port Порт, на якому буде запущено сервер.
     * @param cert_path Шлях до файлу SSL-сертифіката (.crt).
     * @param key_path Шлях до файлу SSL-ключа (.key).
     */
    WebServer(uint16_t port, const std::string& cert_path, const std::string& key_path);

    /**
     * @brief Деструктор (обов'язковий для PIMPL з unique_ptr).
     */
    ~WebServer();

    /**
     * @brief Запускає головний цикл сервера для обробки запитів.
     * Цей метод є блокуючим.
     */
    void run();

private:
    // PIMPL: приховуємо всі деталі реалізації від заголовочного файлу.
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // WEBSERVER_HPP