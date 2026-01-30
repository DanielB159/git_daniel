#include "CryptoUtils.h"
#include <windows.h>
#include <bcrypt.h>

#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

#pragma comment(lib, "bcrypt.lib")


std::string CryptoUtils::sha256_hex(std::string_view input) {
    BCRYPT_ALG_HANDLE hAlg = nullptr;
    BCRYPT_HASH_HANDLE hHash = nullptr;

    DWORD cbData = 0;
    DWORD cbHash = 0;
    DWORD cbHashObject = 0;

    // Open SHA-256 algorithm provider
    NTSTATUS status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr, 0);
    if (status < 0) throw std::runtime_error("BCryptOpenAlgorithmProvider failed");

    auto cleanup = [&] {
        if (hHash) BCryptDestroyHash(hHash);
        if (hAlg)  BCryptCloseAlgorithmProvider(hAlg, 0);
    };

    // Get sizes
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

    // Create hash
    status = BCryptCreateHash(hAlg, &hHash,
                             hashObject.data(), static_cast<ULONG>(hashObject.size()),
                             nullptr, 0, 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptCreateHash failed"); }

    // Hash data
    status = BCryptHashData(hHash,
                            (PUCHAR)input.data(),
                            static_cast<ULONG>(input.size()),
                            0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptHashData failed"); }

    // Finish
    status = BCryptFinishHash(hHash, hash.data(), static_cast<ULONG>(hash.size()), 0);
    if (status < 0) { cleanup(); throw std::runtime_error("BCryptFinishHash failed"); }

    cleanup();

    // Convert to hex
    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(hash.size() * 2);
    for (size_t i = 0; i < hash.size(); ++i) {
        out[2 * i]     = hex[(hash[i] >> 4) & 0xF];
        out[2 * i + 1] = hex[hash[i] & 0xF];
    }
    return out;
}