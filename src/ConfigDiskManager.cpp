#include "CryptoUtils.h"
#include "ConfigDiskManager.h"
#include <filesystem>
#include <iterator>
#include <stdexcept>
#include <string>
#include <system_error>
#include <fstream>
#include <iostream>
#include <cassert>

static void touch_file(const std::filesystem::path& p) {
    std::ofstream ofs(p, std::ios::app);
    if (!ofs) throw std::runtime_error("Failed creating the file: " + p.string());
}

void ConfigDiskManager::initRepoDisk() {
    if (std::filesystem::exists(this->configPath)) {
        std::cout << "repository already initialized, .git_d already exists" << std::endl;
        readIndexFiles();
        return;
    }
    std::error_code ec;
    std::filesystem::create_directory(this->configPath, ec);
    if (ec) throw std::runtime_error("Error in creating .git_d");
    std::filesystem::create_directory(this->configPath / "objects", ec);
    if (ec) throw std::runtime_error("Error in creating the objects dir");
    touch_file(this->configPath / "config");
    touch_file(this->configPath / "index");
    touch_file(this->configPath / "HEAD");
}

ConfigDiskManager::ConfigDiskManager(const std::filesystem::path& repoPath) : configPath(repoPath / ".git_d") {}

ConfigDiskManager& ConfigDiskManager::instance(const std::filesystem::path& repoPath) {
    static ConfigDiskManager inst(repoPath);
    return inst;
}

void ConfigDiskManager::stageFile(const std::filesystem::path& p) {
    std::string fileBlob = this->createBlob(p);
    std::string fileHash = CryptoUtils::sha256_hex(fileBlob);
    this->saveBlobToObjects(fileBlob, fileHash);
    // TODO: should make sure that here or in main.cpp the path is always relative to the root repo
    this->saveBlobHashToIndex(fileHash, p);
}

std::string ConfigDiskManager::createBlob(const std::filesystem::path& p) const {
    std::ifstream file(p, std::ios::binary);
    if (!file) throw std::runtime_error("failed to read the file!");

    std::string content((std::istream_iterator<char>(file)), std::istream_iterator<char>());

    std::string blob = "blob" + std::to_string(content.size()) + '\0' + content;

    // TODO: should compress the blob before returning it

    return blob;
}

void ConfigDiskManager::saveBlobToObjects(const std::string& blob, std::string hash) const {
    std::filesystem::path objectsPath = this->configPath / "objects";
    assert(std::filesystem::exists(objectsPath));
    // save the two chars of the hash as a separate folder - to decrease the amount of objects
    std::string twoChars = hash.substr(0, 2);
    hash.erase(0,2);
    std::filesystem::path blobDir = objectsPath / twoChars;
    if (!std::filesystem::exists(blobDir)) std::filesystem::create_directory(blobDir);
    std::filesystem::path blobPath = blobDir / hash;
    std::ofstream outFile(blobPath, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to create blob file at " + blobPath.string());
    }
    outFile.write(blob.data(), blob.size());
    outFile.close();
}

void ConfigDiskManager::readIndexFiles() {
    assert(indexFiles.empty());
    assert(std::filesystem::exists(this->configPath));
    std::filesystem::path indexPath = this->configPath / "index";
    std::ifstream indexFile(indexPath, std::ios::binary);
    if (!indexFile) {
        throw std::runtime_error("Failed to open index file: " + indexPath.string());
    }
    std::string line;
    std::string::size_type pos;
    while (std::getline(indexFile, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back(); // for windows devices
        if (line.empty()) continue;
        pos = line.find(" ");
        if (pos == std::string::npos) {
            throw std::runtime_error("Failed to parse the index file!");
        }
        std::string pathRelative(line.substr(0, pos));
        std::string latestFileHash(line.substr(pos + 1));
        IndexFile indexFile(pathRelative, latestFileHash);
        this->indexFiles.emplace(pathRelative, indexFile);
    }
}

void ConfigDiskManager::addToIndexFiles(const std::string& pathRelative, const std::string& latestFileHash) {
    auto item = this->indexFiles.find(pathRelative);
    if (item != this->indexFiles.end()) {
        if (item->second.latestFileHash == latestFileHash) return;
    }
    IndexFile indexFile(pathRelative, latestFileHash);
    
    // assign or overwrite the previous one
    this->indexFiles.insert_or_assign(pathRelative, indexFile); 
        
    std::filesystem::path indexPath = this->configPath / "index";
    std::ofstream indexFileStream(indexPath, std::ios::binary | std::ios::trunc);
    if (!indexFileStream) {
        throw std::runtime_error("Failed to open index file for writing: " + indexPath.string());
    }
    for (const auto& [key, fileObj] : this->indexFiles) {
        indexFileStream << fileObj.pathRelative << " " << fileObj.latestFileHash << "\n";
    }
    indexFileStream.close();
}


void ConfigDiskManager::saveBlobHashToIndex(const std::string& latestFileHash, const std::filesystem::path& pathReltive) {
    // TODO: Make sure that all of the paths that are used here are relative to the root repo and not absolute
    this->addToIndexFiles(pathReltive.string(), latestFileHash);
}