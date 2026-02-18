#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include "GitConfig.h"
#include <fstream>

static void touch_file(std::filesystem::path p) {
    std::ofstream ofs(p, std::ios::app);
    if (!ofs) throw std::runtime_error("Failed creating the file: " + p.string());
}

void GitConfig::initRepo(std::filesystem::path &repoPath) {
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

const GitConfig& GitConfig::instance(std::filesystem::path &p) {
    // static initialization makes sure that it's calld only once
    static const GitConfig inst(p);
    return inst;
}

GitConfig::GitConfig(std::filesystem::path &p) {
    if (!std::filesystem::exists(p / ".git_d")) {
        initRepo(p);
    }

}