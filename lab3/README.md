# Hash Hash Hash

This project demonstrates the implementation of thread-safe hash table operations using different locking strategies. It compares the performance of various locking implementations to show the impact of different synchronization approaches on concurrent access.

## Building

To build the program:
```shell
make
```
This will create the executable file.

## Running

The program takes two command-line arguments:
- `-t`: Number of threads to use
- `-s`: Size of the hash table

Example run:
```shell
./hash-table-tester -t 8 -s 100000
```

Expected output:
```
Generation: 158,673 usec
Hash table base: 7,038,725 usec
  - 0 missing
Hash table v1: 10,480,446 usec
  - 0 missing
Hash table v2: 974,497 usec
  - 0 missing
```

## Implementation Details

### First Implementation (v1)
In the `hash_table_v1_add_entry` function, a single global lock is used to protect the entire hash map during entry addition.

#### Performance Analysis
Version 1 is slower than the base version because locking the entire map effectively serializes all operations, negating the benefits of multi-threading. The performance is worse than the base version due to the overhead of lock acquisition and release.

### Second Implementation (v2)
In the `hash_table_v2_add_entry` function, individual locks are used for each bucket in the hash map, allowing concurrent access to different buckets.

#### Performance Analysis
Version 2 shows significant performance improvement because:
- It allows true parallel execution when threads access different buckets
- Reduces lock contention by minimizing the critical section
- Enables better utilization of multiple CPU cores
- Achieves approximately 7x speedup over version 1

## Cleaning up

To remove all binary files and clean the build:
```shell
make clean
```