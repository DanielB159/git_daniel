#pragma once
#include <string>
#include <string_view>


class CryptoUtils {
    public:
        static std::string sha256_hex(std::string_view input);
        CryptoUtils() = delete;
};
