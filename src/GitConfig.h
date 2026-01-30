#pragma once
#include "GitCommit.h"
#include "GitObject.h"
#include <filesystem>
 
class GitConfig {
    
    public:
        static GitConfig& instance(std::filesystem::path& p);
        static void initRepo(std::filesystem::path& p);
    
    private:
        GitConfig(std::filesystem::path&);
        GitConfig* conf;
        std::vector<GitCommit> commitList;
        std::vector<GitObject> currState;
};