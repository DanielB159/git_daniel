#pragma once
#include "GitCommit.h"
#include "GitFolder.h"
#include "GitObject.h"
#include <filesystem>
#include <memory>
 
class GitConfig {
    
    public:
        static GitConfig& instance(const std::filesystem::path&);
        static void initRepo(const std::filesystem::path&);
        bool addGitObj(const std::filesystem::path&);
    
    private:
        GitConfig(const std::filesystem::path&);
        void initFromConfig(const std::filesystem::path&);
        std::shared_ptr<GitObject> contains(const std::filesystem::path&) const;

        GitConfig* conf;
        std::vector<GitCommit> commitList;
        std::shared_ptr<GitFolder> rootObject;
};