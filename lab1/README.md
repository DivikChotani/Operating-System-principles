# Pipe Up

This project implements a command-line tool that creates a pipeline of processes, where the output of each command becomes the input for the next command in the sequence. The program demonstrates process creation, pipe communication, and command execution in Unix-like systems.

## Building

To build the program:
```shell
make
```
This will create the executable file.

## Running

The program takes at least three arguments: the commands to be piped together. Here's an example:

```shell
./pipe ls cat wc
```

This will:
1. Execute `ls` to list files
2. Pipe its output to `cat`
3. Pipe the result to `wc` to count lines, words, and characters

Expected output:
```
      8       8      65
```

## Cleaning up

To remove all binary files and clean the build:
```shell
make clean
```