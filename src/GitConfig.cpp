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

GitConfig::GitConfig(const std::filesystem::path &p) {
    if (!std::filesystem::exists(p / ".git_d")) {
        initRepo(p);
    }
    this->rootObject = std::make_shared<GitFolder>(p);
}

bool GitConfig::addGitObj(const std::filesystem::path& p) {
    
    // TODO: need to add safeguard here to make sure that the path is at least inside of the repo
    
    std::shared_ptr<GitObject> obj = this->contains(p);

    if (obj) { // object already exists by this name

        std::shared_ptr<GitObject> parent = obj->getParentDir();
        std::shared_ptr<GitFolder> parentFolder = std::dynamic_pointer_cast<GitFolder>(parent);
        assert(parentFolder); // make sure this is never nullptr
        return parentFolder->addSubObj(p);

    } else { // object is new to this repo
        
        std::shared_ptr<GitFolder> iterator = this->rootObject;
        
        // find the find the folder containing the path to p
        std::shared_ptr<GitFolder> lastExistingDir = nullptr;
        std::shared_ptr<GitFolder> firstNonExistingDir = nullptr;
        for (auto it = p.begin(); it != p.end(); ++it) {
            if (std::next(it) == p.end()) { // add file/dir to the tree
                iterator->addSubObj(p);
                break;
            }
            
            // if we've already reached a non existing dir, continue adding new dirs
            if (lastExistingDir) {
                std::shared_ptr<GitFolder> newFolder = std::make_shared<GitFolder>(*it, false);
                newFolder->mount(iterator);
                iterator->addSubObj(newFolder);
                iterator = newFolder; 
                continue;
            }

            // try to find the next existing dir
            std::shared_ptr<GitObject> next = iterator->getByName(it->filename().string());
            if (!next) { // mark iterator as last existing dir and continue adding from here
                lastExistingDir = iterator;
                std::shared_ptr<GitFolder> newFolder = std::make_shared<GitFolder>(*it, false);
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