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
        void replaceData();
        explicit GitFolder(const std::filesystem::path&, const bool addSubObj = true);

    private:
        void addSubObjects(const std::filesystem::path&);
        void deleteSubObjects();
        std::vector<std::shared_ptr<GitObject>> subGitObjects;

};