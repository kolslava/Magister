#include "CryptoHelper.hpp"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>

bool SharedLib::CryptoHelper::generateRsaKeyPair(std::string& outPrivateKey, std::string& outPublicKey) {
    // Використовуємо більш сучасний API для генерації
    EVP_PKEY* pkey = EVP_RSA_gen(2048);
    if (!pkey) {
        return false;
    }

    // Створюємо BIO (Memory Buffer) для запису ключів у пам'ять
    BIO* bio_private = BIO_new(BIO_s_mem());
    BIO* bio_public = BIO_new(BIO_s_mem());

    // Записуємо приватний та публічний ключі у форматі PEM
    PEM_write_bio_PrivateKey(bio_private, pkey, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_PUBKEY(bio_public, pkey);

    // Отримуємо вказівники на буфери в пам'яті
    BUF_MEM* mem_private = nullptr;
    BIO_get_mem_ptr(bio_private, &mem_private);
    outPrivateKey.assign(mem_private->data, mem_private->length);

    BUF_MEM* mem_public = nullptr;
    BIO_get_mem_ptr(bio_public, &mem_public);
    outPublicKey.assign(mem_public->data, mem_public->length);

    // Обов'язково звільняємо всю виділену пам'ять
    BIO_free_all(bio_private);
    BIO_free_all(bio_public);
    EVP_PKEY_free(pkey);

    return true;
}