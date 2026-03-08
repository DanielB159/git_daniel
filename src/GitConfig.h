#pragma once
#include "GitCommit.h"
#include "GitFolder.h"
#include "GitObject.h"
#include "ConfigDiskManager.h"
#include <filesystem>
#include <memory>

class GitConfig {
    
    public:
        static GitConfig& instance();
        static bool isInitialized();
        bool addGitObj(const std::filesystem::path&);
        std::shared_ptr<GitFolder> getRoot();
        void printTree() const;
        void stageFile(const std::filesystem::path&) const;
        void stageDir(const std::filesystem::path&) const;
        
    private:
        void initRepo();
        GitConfig(const std::filesystem::path&);
        void initFromConfig(const std::filesystem::path&);
        std::shared_ptr<GitObject> getFromPath(const std::filesystem::path&) const;
        void printGitTreeFromFolder(const std::shared_ptr<GitFolder>&, std::string printPrefix) const;

        ConfigDiskManager& configDiskManager;
        std::vector<GitCommit> commitList;
        std::shared_ptr<GitFolder> rootObject;
};