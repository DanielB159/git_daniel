#pragma once
#include "GitObject.h"
#include <filesystem>
#include <memory>
#include <vector>



class GitFolder : public GitObject {
    public:
        const SubType getType() const { return SubType::SUB_FOLDER; }
        std::shared_ptr<GitObject> getByName(const std::string&) const;
        bool addSubObj(const std::filesystem::path&);
        void addSubObj(std::shared_ptr<GitObject>);
        explicit GitFolder(const std::filesystem::path&, const bool addSubObj = true);

    private:
        
        std::vector<std::shared_ptr<GitObject>> subGitObjects;

};