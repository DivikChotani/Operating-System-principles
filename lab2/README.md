# You Spin Me Round Robin

This project implements a Round Robin process scheduler that simulates the execution of multiple processes with a given time quantum. The scheduler calculates and reports the average waiting time and response time for all processes.

## Building

To build the program:
```shell
make
```
This will create the executable file.

## Running

The program takes two arguments:
1. A file containing process information (processes.txt)
2. The time quantum for the Round Robin scheduler

```shell
./rr processes.txt 3
```

Expected output:
```
Average waiting time: 7.00
Average response time: 2.75
```

Alternatively, you can run the test suite:
```shell
python -m unittest
```

Expected test output:
```
..
----------------------------------------------------------------------
Ran 2 tests in 4.371s

OK
```

## Cleaning up

To remove all binary files and clean the build:
```shell
make clean
```
