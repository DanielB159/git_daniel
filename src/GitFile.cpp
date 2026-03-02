#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include "GitFile.h"
#include "FileChunk.h"

GitFile::GitFile(const std::filesystem::path& p) : GitObject(p.filename().string()) {
    parseChunks(p);
}

void GitFile::parseChunks(const std::filesystem::path& p) {
    std::ifstream in(p, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to read file at path: " + p.string());
    std::string chunk_text = "";
    std::string line;
    int lines_passed = 0;
    while (std::getline(in, line)) {
        chunk_text += line;
        ++lines_passed;

        if (lines_passed >= this->MAX_CHUNK_SIZE) {
            FileChunk chunk(std::move(chunk_text));
            this->chunks.push_back(std::move(chunk));
            chunk_text = "";
            lines_passed = 0;
        }
    }
    if (!chunk_text.empty()) {
        FileChunk chunk(std::move(chunk_text));
        this->chunks.push_back(std::move(chunk));
    }
}

void GitFile::replaceData() { // later on should index the changes to the commits in GitConfig
    this->chunks.clear();
    this->chunks.shrink_to_fit();
    this->parseChunks(this->path);
}