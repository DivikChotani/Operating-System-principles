# Hey! I'm Filing Here

This project implements a file system simulator that demonstrates various file system operations including file creation, deletion, and management of file metadata. The implementation includes features like:
- File allocation strategies
- Directory management
- File system consistency checks
- Space management

## Building

To build the program:
```shell
make
```
This will create the executable file.

## Running

The program can be run with various command-line options to test different file system operations. Example usage:

```shell
./fs-simulator [options]
```

Common options include:
- `-c`: Create a new file
- `-d`: Delete a file
- `-l`: List directory contents
- `-s`: Show file system statistics

## Cleaning up

To remove all binary files and clean the build:
```shell
make clean
```
