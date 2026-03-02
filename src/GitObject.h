#pragma once
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>

class GitObject : public std::enable_shared_from_this<GitObject> {
    public:
        enum SubType {
            SUB_FILE,
            SUB_FOLDER
        };
        virtual const SubType getType() const = 0;
        virtual bool replaceData(const std::filesystem::path&) = 0;
        virtual void mount(std::shared_ptr<GitObject> parent) final { this->parentDir = parent; };
        virtual const bool isSameType(const GitObject* other) const final { return this->getType() == other->getType(); }
        const bool isDirectory() const { return this->getType() == SubType::SUB_FOLDER; }
        const std::string& getName() const { return this->name; }
        std::shared_ptr<GitObject> getParentDir() const { return this->parentDir; }
        virtual ~GitObject() = default;

    protected:
        
        GitObject() = default;
        explicit GitObject(const std::filesystem::path& path) : path(path), name(path.filename().string()) {}
        std::string name;
        const std::filesystem::path& path;

    private:
        std::shared_ptr<GitObject> parentDir = nullptr;
};