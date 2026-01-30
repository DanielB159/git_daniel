#include "GitConfig.h"
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: git_d <init|add|commit>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        std::filesystem::path repoPath = std::filesystem::current_path();
        if (argc >= 3) {
            repoPath = argv[2];
        }
        try {
            GitConfig::initRepo(repoPath);
        } catch (const std::exception& ex) {
            std::cerr << "Init failed: " << ex.what() << std::endl;
        }
        return 0;
    }

    if (command == "add") {
        std::cout << "add" << std::endl;
        return 0;
    }

    if (command == "commit") {
        std::cout << "commit" << std::endl;
        return 0;
    }

    std::cerr << "Unknown command: " << command << std::endl;
    return 1;
}
