#pragma once

#include <string>

namespace SharedLib {

    class SignatureManager {
    public:
        static bool sign(const std::string& privateKeyPem, const std::string& dataToSign, std::string& outSignatureBase64);
        static bool verify(const std::string& publicKeyPem, const std::string& dataToVerify, const std::string& signatureBase64);
    };

} // namespace Shared