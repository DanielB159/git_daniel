#pragma once
#include <vector>
#include <filesystem>
#include "GitObject.h"
#include "FileChunk.h"

class GitFile : public GitObject {

    public:
        explicit GitFile(const std::filesystem::path&);
        virtual const SubType getType() const { return SubType::SUB_FILE; }
        void replaceData();
    private:
        const uint8_t MAX_CHUNK_SIZE = 5;

        void parseChunks(const std::filesystem::path&);
        std::vector<FileChunk> chunks;

};