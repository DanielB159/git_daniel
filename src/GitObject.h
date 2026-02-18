#pragma once

class GitObject {
    public:
        virtual void addObject() = 0;
        virtual ~GitObject() = default;
};