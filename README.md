# Codexion

*This project has been created as part of the 42 curriculum by azebahad.*

## Description
Codexion simulates coders competing for shared USB dongles while compiling, debugging, and refactoring under concurrency constraints. The program uses POSIX threads, mutexes, condition variables, and a simple priority heap to enforce fairness and prevent deadlocks.

## Instructions
Compile with:

```sh
make
```

Run the simulator with:

```sh
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <fifo|edf>
```

## Blocking cases handled
- Deadlock avoidance by acquiring dongles in a deterministic order.
- Cooldown enforcement to prevent immediate re-acquisition of released dongles.
- Fair arbitration with FIFO or EDF scheduling via a custom heap.
- Precise burnout detection and coordinated shutdown.
- Serialized logging with a dedicated mutex.

## Thread synchronization mechanisms
The implementation uses pthread mutexes and condition variables to protect dongle state, queue requests, and serialize output. Each coder thread only enters the compile phase after acquiring both resources, which prevents races and inconsistent states.

## Resources
- POSIX Threads documentation
- pthread mutex and condition variable tutorials
- Concurrency notes on deadlocks, starvation, and scheduling
