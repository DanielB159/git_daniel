#pragma once
#include <string>

class FileChunk {
    public:
        explicit FileChunk(std::string&& text) noexcept;
        explicit FileChunk(FileChunk&& chunk) noexcept;
        FileChunk& operator=(FileChunk&& other) noexcept;
        FileChunk(std::string&) = delete;
        const std::string& getHash() const noexcept;
        const std::string& getText() const noexcept;
    
    private:
        void calcHash();
        std::string text;
        std::string hash;
};