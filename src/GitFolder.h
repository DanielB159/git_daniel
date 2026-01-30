#pragma once
#include "GitFile.h"
#include "GitObject.h"
#include <vector>

class GirFolder : GitObject {
    public:
        void addObject();

    private:
        std::vector<GitFile> files;
};