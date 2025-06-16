/*

*/

#pragma once

#define CRYPTOHELPER_HPP
#ifdef CRYPTOHELPER_HPP

#include <string>

namespace SharedLib {
    /**
     * @brief Допоміжний клас, що відповідає виключно за генерацію криптографічних ключів.
     */
    class CryptoHelper {
    public:
        /**
         * @brief Генерує нову пару RSA-ключів (2048 біт) у форматі PEM.
         * @param outPrivateKey Рядок, в який буде збережено приватний ключ.
         * @param outPublicKey Рядок, в який буде збережено публічний ключ.
         * @return true у разі успіху.
         */
        static bool generateRsaKeyPair(std::string& outPrivateKey, std::string& outPublicKey);
    };

}

#endif // CRYPTOHELPER_HPP