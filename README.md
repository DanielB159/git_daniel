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


# Compiling and running
- In the first time compiling and in every addition of source file run this command that generates the ninja files to build the project:
```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
```

- When the ninja files to build are already existing, run this command to build an exe:
```
cmake --build build
```

- In order to run the executable:
```
./build/git_d
```