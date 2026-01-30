#pragma once
#include <vector>
#include <filesystem>
#include "GitObject.h"
#include "FileChunk.h"

class GitFile : GitObject {

    public:
        void addObject();
        explicit GitFile(std::filesystem::path&);
    private:
        const uint8_t MAX_CHUNK_SIZE = 5;

        void parseChunks(std::filesystem::path&);
        std::vector<FileChunk> chunks;

};