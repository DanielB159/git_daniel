#pragma once
#include <string>
#include <vector>

class FileChunk {
    public:
        void calcHash();
        std::string getHash();
        FileChunk(std::string text);

    private:
        std::vector<std::string> text;
        std::string hash;
};