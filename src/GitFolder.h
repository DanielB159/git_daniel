#pragma once
#include "GitObject.h"
#include <filesystem>
#include <memory>
#include <vector>



class GitFolder : public GitObject {
    public:
        static std::shared_ptr<GitFolder> create(const std::filesystem::path&, bool populate = true);
        const SubType getType() const { return SubType::SUB_FOLDER; }
        std::shared_ptr<GitObject> getByName(const std::string&) const;
        const std::vector<std::shared_ptr<GitObject>>& getSubObjects() const { return subGitObjects; }
        bool addSubObj(const std::filesystem::path&);
        void addSubObj(std::shared_ptr<GitObject>);
        void replaceData();
        explicit GitFolder(const std::filesystem::path&);

    private:
        void addSubObjects(const std::filesystem::path&);
        void deleteSubObjects();
        std::vector<std::shared_ptr<GitObject>> subGitObjects;

};