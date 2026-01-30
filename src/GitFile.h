#pragma once
#include "GitObject.h"
#include "FileChunk.h"
#include <vector>

class GitFile : GitObject {

    public:
        void addObject();
    private:
        std::vector<FileChunk> chunks;
};