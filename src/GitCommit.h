#pragma once
#include <string>
#include <vector>
#include "GitObject.h"

class GitCommit{

    private:
        std::vector<GitObject> objectsChanged;
        std::string commitHash;
};