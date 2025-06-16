#include "SignatureManager.hpp"
#include "Logger.hpp"
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <vector>

namespace {
    // Допоміжна функція для кодування в Base64
    bool toBase64(const std::vector<unsigned char>& input, std::string& output) {
        BIO *b64 = BIO_new(BIO_f_base64()), *bmem = BIO_new(BIO_s_mem());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO_push(b64, bmem);
        if(BIO_write(b64, input.data(), input.size()) <= 0) {
            BIO_free_all(b64);
            return false;
        }
        BIO_flush(b64);
        BUF_MEM* bptr = NULL;
        BIO_get_mem_ptr(b64, &bptr);
        output.assign(bptr->data, bptr->length);
        BIO_free_all(b64);
        return true;
    }

    // Допоміжна функція для декодування з Base64
    bool fromBase64(const std::string& input, std::vector<unsigned char>& output) {
        BIO *b64 = BIO_new(BIO_f_base64()), *bmem = BIO_new_mem_buf((void*)input.c_str(), input.length());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO_push(b64, bmem);
        output.resize(input.length());
        int decoded_size = BIO_read(b64, output.data(), output.size());
        BIO_free_all(b64);
        if (decoded_size < 0) return false;
        output.resize(decoded_size);
        return true;
    }
}

namespace SharedLib {

bool SignatureManager::sign(const std::string& privateKeyPem, const std::string& dataToSign, std::string& outSignatureBase64) {
    BIO* bio = BIO_new_mem_buf(privateKeyPem.c_str(), -1);
    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    BIO_free_all(bio);
    if (!pkey) { Logger::error("Failed to load private key."); return false; }

    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) { EVP_PKEY_free(pkey); return false; }

    if (EVP_DigestSignInit(md_ctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }
    if (EVP_DigestSignUpdate(md_ctx, dataToSign.c_str(), dataToSign.length()) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }
    size_t sig_len;
    if (EVP_DigestSignFinal(md_ctx, NULL, &sig_len) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }
    std::vector<unsigned char> signature(sig_len);
    if (EVP_DigestSignFinal(md_ctx, signature.data(), &sig_len) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }

    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(pkey);
    return toBase64(signature, outSignatureBase64);
}

bool SignatureManager::verify(const std::string& publicKeyPem, const std::string& dataToVerify, const std::string& signatureBase64) {
    BIO* bio = BIO_new_mem_buf(publicKeyPem.c_str(), -1);
    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free_all(bio);
    if (!pkey) { Logger::error("Failed to load public key."); return false; }
    
    std::vector<unsigned char> signature;
    if (!fromBase64(signatureBase64, signature)) {
        Logger::error("Failed to decode base64 signature."); EVP_PKEY_free(pkey); return false;
    }

    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) { EVP_PKEY_free(pkey); return false; }
    
    if (EVP_DigestVerifyInit(md_ctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }
    if (EVP_DigestVerifyUpdate(md_ctx, dataToVerify.c_str(), dataToVerify.length()) <= 0) {
        EVP_MD_CTX_free(md_ctx); EVP_PKEY_free(pkey); return false;
    }

    int result = EVP_DigestVerifyFinal(md_ctx, signature.data(), signature.size());
    
    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(pkey);
    
    return result == 1;
}

} // namespace Shared