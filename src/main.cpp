#include "GitConfig.h"
#include "GitFile.h"
#include "GitFolder.h"
#include "GitObject.h"
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>


GitConfig& getRepoConfig() {
    std::filesystem::path currPath = std::filesystem::current_path();
    return GitConfig::instance(currPath);
}

void printGitTree(std::shared_ptr<GitFolder> root, const std::string& prefix = "", bool isLast = true) {
    std::cout << prefix << (isLast ? "└── " : "├── ") << root->getName() << "/" << std::endl;
    const auto& children = root->getSubObjects();
    for (size_t i = 0; i < children.size(); ++i) {
        const auto& child = children[i];
        const bool last = (i == children.size() - 1);
        const std::string childPrefix = prefix + (isLast ? "    " : "│   ");
        if (child->isDirectory()) {
            printGitTree(std::dynamic_pointer_cast<GitFolder>(child), childPrefix, last);
        } else {
            std::cout << childPrefix << (last ? "└── " : "├── ") << child->getName() << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: git_d <init|add|commit>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        std::filesystem::path repoPath = std::filesystem::current_path();
        if (argc >= 3) {
            std::cerr << "init currently does not support dynamic paths" << std::endl;
            // repoPath = argv[2];
        }
        try {
            GitConfig::initRepo(repoPath);
        } catch (const std::exception& ex) {
            std::cerr << "Init failed: " << ex.what() << std::endl;
        }
        return 0;
    }

    if (command == "add") {

        do {
            if (argc < 3) {
                std::cerr << "add failed. Please provide a file or folder to add" << std::endl;
                break;
            }
            std::filesystem::path addPath = argv[2];
            if (!std::filesystem::exists(addPath)) {
                std::cerr << "add failed. Please provide a valid path" << std::endl;
                break;
            }
            addPath = std::filesystem::canonical(addPath);
            GitConfig& config = getRepoConfig(); 
            if (!config.addGitObj(addPath)) {
                std::cerr << "Adding this path to git failed!" << std::endl;
            }
            printGitTree(config.getRoot());

        } while (false);

        return 0;
    }

    if (command == "commit") {
        std::cout << "commit" << std::endl;
        return 0;
    }

    std::cerr << "Unknown command: " << command << std::endl;
    return 1;
}
