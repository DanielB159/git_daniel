#include "GitFolder.h"
#include "GitFile.h"
#include "GitObject.h"
#include <cassert>
#include <filesystem>
#include <memory>
#include <iostream>

std::shared_ptr<GitFolder> GitFolder::create(const std::filesystem::path& p, bool populate) {
    auto folder = std::make_shared<GitFolder>(p);
    if (populate) folder->addSubObjects(p);
    return folder;
}

GitFolder::GitFolder(const std::filesystem::path& p) : GitObject(p) {
    assert(std::filesystem::is_directory(p));
}

void GitFolder::addSubObjects(const std::filesystem::path& p) {
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        const auto& subPath = entry.path();
        std::shared_ptr<GitObject> newEntry;
        if (entry.is_directory()) {
            newEntry = GitFolder::create(subPath);
        } else if (entry.is_regular_file()) {
            newEntry = std::make_shared<GitFile>(subPath);
        }
        newEntry->mount(shared_from_this());
        this->addSubObj(newEntry);
    }
}

std::shared_ptr<GitObject> GitFolder::getByName(const std::string& name) const {
    for (const auto& gitObj : this->subGitObjects) {
        if (gitObj->getName() == name) {
            return gitObj;
        }
    }
    return nullptr;
}

bool GitFolder::addSubObj(const std::filesystem::path& p) {

    if (p.parent_path() != this->path) {
        return false;
    }

    std::shared_ptr<GitObject> obj = this->getByName(p.filename().string());
    const bool isDirectory = std::filesystem::is_directory(p);

    if (obj) {
        if (isDirectory != obj->isDirectory()) {
            std::cerr << this->path.filename().string() << "already contains a " << 
                    (obj->isDirectory() ? "directory" : "file") << " with this name." << std::endl;
            return false;
        }
        obj->replaceData();
        return true;
    }

    std::shared_ptr<GitObject> newObj;
    if (!isDirectory) {
        assert(std::filesystem::is_regular_file(p));
        newObj = std::make_shared<GitFile>(p);
    } else {
        assert(std::filesystem::is_directory(p));
        newObj = GitFolder::create(p);
    }
    
    newObj->mount(shared_from_this());
    this->addSubObj(newObj);
    return true;
}

void GitFolder::addSubObj(std::shared_ptr<GitObject> obj) {
    this->subGitObjects.push_back(obj);
}

void GitFolder::deleteSubObjects() {
    this->subGitObjects.clear();
}

void GitFolder::replaceData() {
    this->deleteSubObjects();
    this->addSubObjects(this->path);
}


