# git_daniel
This repo contains an implementation of a few git functionalities for self learning.
As it's hosted in github and requires git features by itself, this git will work when invoking git_d.

# Functionalities
## git_d init
- Initialize a directory .git_d which would have the data structures that power git persisted in disk.

## git_d add <filename>
- Add changes in a file to all of the staged files. If the file did not change since it's last state, will return this output

## git_d commit -m "<git message>"
- Commit all of the staged changes to the working tree.

# Project structure
```
git_daniel/
├── src/            # Shared source code (all .cpp and .h files)
├── windows/        # CMakeLists.txt for building on Windows
└── unix/           # CMakeLists.txt for building on macOS / Linux
```

The source code in `src/` is shared across platforms. Platform-specific SHA-256
hashing is handled via preprocessor guards in `CryptoUtils.cpp` (Windows BCrypt,
macOS CommonCrypto, Linux OpenSSL).

Requires **C++23**.

# Building and running

## macOS / Linux (Unix)

All commands are run from the repository root.

### First-time setup (generates Makefiles)
```
cmake -S unix -B unix/build -DCMAKE_CXX_COMPILER=clang++
```

### Build
```
cmake --build unix/build
```

### Run
```
./unix/build/git_d
```

> **Linux note:** OpenSSL development headers are required (`libssl-dev` on
> Debian/Ubuntu, `openssl-devel` on Fedora/RHEL).

## Windows

All commands are run from the repository root.

### First-time setup (generates Ninja build files)
```
cmake -S windows -B windows/build -G Ninja -DCMAKE_CXX_COMPILER=clang++
```

### Build
```
cmake --build windows/build
```

### Run
```
.\windows\build\git_d.exe
```