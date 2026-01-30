#pragma once
#include "GitCommit.h"
#include "GitObject.h"
#include <filesystem>
 
class GitConfig {
    
    public:
        static GitConfig& instance(std::filesystem::path&);
        static void initRepo(std::filesystem::path&);
    
    private:
        GitConfig(std::filesystem::path&);
        void initFromConfig(std::filesystem::path&);

        GitConfig* conf;
        std::vector<GitCommit> commitList;
        std::vector<GitObject> currState;
};