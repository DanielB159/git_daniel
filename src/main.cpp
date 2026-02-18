#include "GitConfig.h"
#include "GitFile.h"
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>


const GitConfig& getRepoConfig() {
    std::filesystem::path currPath = std::filesystem::current_path();
    return GitConfig::instance(currPath);
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
            const GitConfig config = getRepoConfig(); // to be used to add the file 
            if (std::filesystem::is_directory(addPath)) {

            } else if (std::filesystem::is_regular_file(addPath)) {
                GitFile file = new GitFile(addPath);
            } else {
                std::cerr << "add failed. The path isn't a regular file or directory" << std::endl;
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
