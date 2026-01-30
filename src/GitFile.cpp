#include <fstream>
#include <stdexcept>
#include <string>
#include "GitFile.h"
#include "FileChunk.h"

GitFile::GitFile(std::filesystem::path& p) {
    parseChunks(p);
}

void GitFile::parseChunks(std::filesystem::path& p) {
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