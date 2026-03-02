#include "FileChunk.h"
#include "CryptoUtils.h"
#include <cstdio>
#include <string>

FileChunk::FileChunk(std::string&& text) noexcept: text(std::move(text)) {
    calcHash();
}

FileChunk::FileChunk(FileChunk&& other) noexcept: text(std::move(other.text)), hash(std::move(other.hash)) {}

FileChunk& FileChunk::operator=(FileChunk&& other) noexcept {
    if (this != &other) {
        this->text = std::move(other.text);
        this->hash = std::move(other.hash);
    }
    return *this;
}

const std::string& FileChunk::getHash() const noexcept {
    return this->hash;
}

const std::string& FileChunk::getText() const noexcept{
    return this->text;
}

void FileChunk::calcHash() {
    this->hash = CryptoUtils::sha256_hex(this->text);
}