#pragma once
#include <vector>
#include <filesystem>
#include "GitObject.h"
#include "FileChunk.h"

class GitFile : public GitObject {

    public:
        explicit GitFile(std::filesystem::path&);
        virtual const SubType getType() const { return SubType::SUB_FILE; }
    private:
        const uint8_t MAX_CHUNK_SIZE = 5;

        void parseChunks(std::filesystem::path&);
        std::vector<FileChunk> chunks;

};