#include "CryptoUtils.h"

#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
// ── Windows: BCrypt ──────────────────────────────────────────────────────────
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

std::string CryptoUtils::sha256_hex(std::string_view input) {
    BCRYPT_ALG_HANDLE hAlg = nullptr;
    BCRYPT_HASH_HANDLE hHash = nullptr;

    DWORD cbData = 0;
    DWORD cbHash = 0;
    DWORD cbHashObject = 0;

    NTSTATUS status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr, 0);
    if (status < 0) throw std::runtime_error("BCryptOpenAlgorithmProvider failed");

    auto cleanup = [&] {
        if (hHash) BCryptDestroyHash(hHash);
        if (hAlg)  BCryptCloseAlgorithmProvider(hAlg, 0);
    };

    status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH,
                              reinterpret_cast<PUCHAR>(&cbHashObject), sizeof(cbHashObject),
                              &cbData, 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptGetProperty(OBJECT_LENGTH) failed"); }

    status = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH,
                              reinterpret_cast<PUCHAR>(&cbHash), sizeof(cbHash),
                              &cbData, 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptGetProperty(HASH_LENGTH) failed"); }

    std::vector<unsigned char> hashObject(cbHashObject);
    std::vector<unsigned char> hash(cbHash);

    status = BCryptCreateHash(hAlg, &hHash,
                             hashObject.data(), static_cast<ULONG>(hashObject.size()),
                             nullptr, 0, 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptCreateHash failed"); }

    status = BCryptHashData(hHash,
                            (PUCHAR)input.data(),
                            static_cast<ULONG>(input.size()),
                            0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptHashData failed"); }

    status = BCryptFinishHash(hHash, hash.data(), static_cast<ULONG>(hash.size()), 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptFinishHash failed"); }

    cleanup();

    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(hash.size() * 2);
    for (size_t i = 0; i < hash.size(); ++i) {
        out[2 * i]     = hex[(hash[i] >> 4) & 0xF];
        out[2 * i + 1] = hex[hash[i] & 0xF];
    }
    return out;
}

#elif defined(__APPLE__)
// ── macOS: CommonCrypto ──────────────────────────────────────────────────────
#include <CommonCrypto/CommonDigest.h>

std::string CryptoUtils::sha256_hex(std::string_view input) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(input.data(), static_cast<CC_LONG>(input.size()), hash);

    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(CC_SHA256_DIGEST_LENGTH * 2);
    for (size_t i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) {
        out[2 * i]     = hex[(hash[i] >> 4) & 0xF];
        out[2 * i + 1] = hex[hash[i] & 0xF];
    }
    return out;
}

#else
// ── Linux / other Unix: OpenSSL ──────────────────────────────────────────────
#include <openssl/evp.h>

std::string CryptoUtils::sha256_hex(std::string_view input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_MD_CTX_new failed");

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx, input.data(), input.size()) != 1 ||
        EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("SHA-256 computation failed");
    }
    EVP_MD_CTX_free(ctx);

    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(hash_len * 2);
    for (unsigned int i = 0; i < hash_len; ++i) {
        out[2 * i]     = hex[(hash[i] >> 4) & 0xF];
        out[2 * i + 1] = hex[hash[i] & 0xF];
    }
    return out;
}

#endif