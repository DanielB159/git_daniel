#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GitObject.h"

class GitCommit{

    private:
        std::vector<std::unique_ptr<GitObject>> objectsChanged;
        std::string commitHash;
};