#include "GitConfig.h"
#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: git_d <init|add|commit>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        if (argc >= 3) {
            std::cerr << "init currently does not support dynamic paths" << std::endl;
        }
        try {
            GitConfig::instance(); // initializes the repo and creates the git folders in the repoPath
        } catch (const std::exception& ex) {
            std::cerr << "Init failed: " << ex.what() << std::endl;
        }
        return 0;
    }

    if (!GitConfig::isInitialized()) {
        std::cerr << "The repository is not initialized. Please init before doing any git acion" << std::endl;
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
            if (addPath.is_absolute()) {
                std::cerr << "git only supports relative paths from the root repo" << std::endl;
            }
            if (std::filesystem::is_directory(addPath)) {
                GitConfig::instance().stageDir(addPath);
            } else {
                GitConfig::instance().stageFile(addPath);
            }

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
