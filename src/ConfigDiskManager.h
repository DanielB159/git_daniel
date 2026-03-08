#pragma once
#include <filesystem>
#include <string>
#include <map>


struct IndexFile {
    std::string pathRelative;
    std::string latestFileHash;
    IndexFile(std::string _pathRelative, std::string _latestFileHash): pathRelative(_pathRelative),latestFileHash(_latestFileHash) {}
};


class ConfigDiskManager {

    public:
        static ConfigDiskManager& instance(const std::filesystem::path&);
        void initRepoDisk(); // file to init and / or initialize all git objects
        void stageFile(const std::filesystem::path&);

    private:
        std::string createBlob(const std::filesystem::path&) const;
        void saveBlobToObjects(const std::string&, std::string) const;
        void saveBlobHashToIndex(const std::string&, const std::filesystem::path&);
        ConfigDiskManager(const std::filesystem::path&);
        void readIndexFiles();
        void addToIndexFiles(const std::string&, const std::string&);
        const std::filesystem::path configPath;
        std::map<std::string, IndexFile> indexFiles;
};