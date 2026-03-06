#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <fstream>
#include <cassert>
#include "GitConfig.h"
#include "GitFolder.h"
#include "GitObject.h"

static void touch_file(std::filesystem::path p) {
    std::ofstream ofs(p, std::ios::app);
    if (!ofs) throw std::runtime_error("Failed creating the file: " + p.string());
}

std::shared_ptr<GitFolder> GitConfig::getRoot() {
    return this->rootObject;
}

void GitConfig::initRepo(const std::filesystem::path &repoPath) {
    if (!std::filesystem::exists(repoPath)) throw std::runtime_error("Repository path not found");
    std::filesystem::path gitConfigPath = repoPath / ".git_d";
    if (std::filesystem::exists(gitConfigPath)) {
        std::cout << "repository already initialized, .git_d already exists" << std::endl;
        return;
    }
    std::error_code ec;
    std::filesystem::create_directory(gitConfigPath, ec);
    if (ec) throw std::runtime_error("Error in creating .git_d");
    std::filesystem::create_directory(gitConfigPath / "config", ec);
    if (ec) throw std::runtime_error("Error in creating .git_d/config");
    touch_file(gitConfigPath / "index");
    touch_file(gitConfigPath / "HEAD");
}

GitConfig& GitConfig::instance(const std::filesystem::path &p) {
    // static initialization makes sure that it's calld only once
    static GitConfig inst(p);
    return inst;
}

std::shared_ptr<GitObject> GitConfig::getFromPath(const std::filesystem::path& p) const {
    std::shared_ptr<GitFolder> iterator = this->rootObject;

    for (auto it = p.begin(); it != p.end(); ++it) {
        if (std::next(it) == p.end()) break;
        std::shared_ptr<GitObject> obj = iterator->getByName(it->string());
        if (obj && obj->isDirectory()) {
            iterator = std::dynamic_pointer_cast<GitFolder>(obj);
        } else {
            return nullptr;
        }
    }

    return iterator->getByName(p.filename().string());

}

GitConfig::GitConfig(const std::filesystem::path &p) {
    if (!std::filesystem::exists(p / ".git_d")) {
        initRepo(p);
    }
    this->rootObject = GitFolder::create(p, false);
}

bool GitConfig::addGitObj(const std::filesystem::path& p) {
    
    // TODO: need to add safeguard here to make sure that the path is at least inside of the repo
    // p is expected to be a canonical absolute path (caller's responsibility)
    const std::filesystem::path repoRoot = this->rootObject->getPath();
    const std::filesystem::path relPath = p.lexically_relative(repoRoot);

    std::shared_ptr<GitObject> obj = this->getFromPath(relPath);

    if (obj) { // object already exists by this name

        std::shared_ptr<GitObject> parent = obj->getParentDir();
        std::shared_ptr<GitFolder> parentFolder = std::dynamic_pointer_cast<GitFolder>(parent);
        assert(parentFolder); // make sure this is never nullptr
        return parentFolder->addSubObj(p);

    } else { // object is new to this repo
        
        std::shared_ptr<GitFolder> iterator = this->rootObject;
        
        // iterate relative segments, but create objects with full absolute paths
        std::shared_ptr<GitFolder> lastExistingDir = nullptr;
        std::shared_ptr<GitFolder> firstNonExistingDir = nullptr;
        std::filesystem::path cumulativePath = repoRoot;
        for (auto it = relPath.begin(); it != relPath.end(); ++it) {
            cumulativePath /= *it;
            if (std::next(it) == relPath.end()) { // add file/dir to the tree
                iterator->addSubObj(p);
                break;
            }
            
            // if we've already reached a non existing dir, continue adding new dirs
            if (lastExistingDir) {
                std::shared_ptr<GitFolder> newFolder = GitFolder::create(cumulativePath, false);
                newFolder->mount(iterator);
                iterator->addSubObj(newFolder);
                iterator = newFolder; 
                continue;
            }

            // try to find the next existing dir
            std::shared_ptr<GitObject> next = iterator->getByName(it->string());
            if (!next) { // mark iterator as last existing dir and continue adding from here
                lastExistingDir = iterator;
                std::shared_ptr<GitFolder> newFolder = GitFolder::create(cumulativePath, false);
                newFolder->mount(iterator);
                firstNonExistingDir = newFolder;
                iterator = newFolder;
                continue;
            }
            if (!next->isDirectory()) return false;
            
            assert(next->isDirectory());
            iterator = std::dynamic_pointer_cast<GitFolder>(next);
        }

        // if the for loop completed and contained new dirs, mount this sub-tree to the main tree
        if (lastExistingDir) {
            lastExistingDir->addSubObj(firstNonExistingDir);
        }
    }

    return true;
}

void GitConfig::printTree() const {
    this->printGitTreeFromFolder(this->rootObject, "");
}

void GitConfig::printGitTreeFromFolder(const std::shared_ptr<GitFolder>& root, std::string printPrefix) const {
    for (auto& subObj : root->getSubObjects()) {
        if (subObj->isDirectory()) {
            std::cout << printPrefix << subObj->getName() << ":" << std::endl;
            this->printGitTreeFromFolder(std::dynamic_pointer_cast<GitFolder>(subObj), printPrefix + "|- ");
        } else {
            std::cout << printPrefix << subObj->getName() << std::endl;
        }
    }
}