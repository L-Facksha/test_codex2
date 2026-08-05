# Pthreads (POSIX Threads)

> Threads are a concept provided by the operating system.
> **Pthreads** is the standard C library that allows us to create, manage, synchronize, and coordinate threads on POSIX-compliant systems such as Linux and macOS.

---

# Introduction

In the previous chapter, we learned the theory behind synchronization.

We studied concepts such as:

- Threads
- Race Conditions
- Critical Sections
- Mutexes
- Semaphores
- Deadlocks

However, understanding the theory is only the first step.

To write multithreaded programs, we need an API that allows us to interact with the operating system.

On Unix-like systems, that API is **POSIX Threads**, commonly known as **Pthreads**.

Pthreads is a standardized library defined by the POSIX specification.

It provides the functions needed to:

- Create threads
- Wait for threads
- Synchronize threads
- Protect shared resources
- Coordinate concurrent execution

Almost every multithreaded C program on Linux uses Pthreads.

---

# What is Pthreads?

Pthreads is a C library that provides an interface for working with threads.

Instead of creating processes, Pthreads allows a program to create multiple execution paths inside the same process.

```
Process

    │

    ├────────► Thread 1

    ├────────► Thread 2

    ├────────► Thread 3

    └────────► Thread 4
```

Each thread executes independently while sharing the process's memory.

---

# Why Use Pthreads?

Without Pthreads, a C program executes sequentially.

```
Task A

↓

Task B

↓

Task C
```

With Pthreads,

tasks can execute concurrently.

```
Task A

Task B

Task C
```

This improves:

- Performance
- Responsiveness
- CPU utilization
- Parallel execution on multi-core processors

---

# Compiling a Pthreads Program

Programs that use Pthreads must be linked with the pthread library.

Using GCC:

```bash
gcc main.c -pthread
```

The `-pthread` option enables thread support and links the required library.

---

# Core Components of Pthreads

The most commonly used components include:

### Thread Management

- `pthread_create()`
- `pthread_join()`
- `pthread_exit()`
- `pthread_detach()`
- `pthread_self()`
- `pthread_equal()`

---

### Thread Attributes

- `pthread_attr_init()`
- `pthread_attr_destroy()`
- `pthread_attr_setdetachstate()`
- `pthread_attr_getdetachstate()`

---

### Mutexes

- `pthread_mutex_init()`
- `pthread_mutex_lock()`
- `pthread_mutex_unlock()`
- `pthread_mutex_destroy()`

---

### Condition Variables

- `pthread_cond_init()`
- `pthread_cond_wait()`
- `pthread_cond_signal()`
- `pthread_cond_broadcast()`
- `pthread_cond_destroy()`

---

### Read-Write Locks

- `pthread_rwlock_init()`
- `pthread_rwlock_rdlock()`
- `pthread_rwlock_wrlock()`
- `pthread_rwlock_unlock()`

---

### Barriers

- `pthread_barrier_init()`
- `pthread_barrier_wait()`
- `pthread_barrier_destroy()`

---

# Thread Lifecycle

A typical Pthreads program follows this sequence:

```
Main Thread

↓

Create Threads

↓

Threads Execute

↓

Synchronize

↓

Threads Finish

↓

Join Threads

↓

Program Ends
```

---

# Passing Data to Threads

Each thread can receive an argument when it is created.

```c
pthread_create(&thread,
               NULL,
               worker,
               &data);
```

The argument is passed as a `void *`, allowing any data type to be supplied after casting.

This makes thread functions flexible and reusable.

---

# Returning Data from Threads

A thread can return a value when it finishes.

The calling thread retrieves it using:

```c
pthread_join(thread, &result);
```

This allows one thread to compute a result while another waits for it.

---

# Joinable vs Detached Threads

Pthreads supports two types of threads.

### Joinable Threads

- Default behavior.
- Another thread can wait for them using `pthread_join()`.
- Resources are released only after joining.

---

### Detached Threads

- Independent.
- Cannot be joined.
- Resources are released automatically when the thread exits.

Detached threads are useful for background tasks that no other thread needs to wait for.

---

# Synchronization in Pthreads

Because threads share memory, synchronization is essential.

Pthreads provides several synchronization primitives:

- Mutexes
- Condition Variables
- Read-Write Locks
- Barriers
- Spinlocks (platform-dependent)

These tools help prevent race conditions and coordinate concurrent execution.

---

# Common Mistakes

When learning Pthreads, beginners often encounter issues such as:

- Forgetting to join threads.
- Joining detached threads.
- Forgetting to unlock a mutex.
- Passing invalid pointers to threads.
- Returning pointers to local variables.
- Creating race conditions by accessing shared data without synchronization.
- Deadlocks caused by incorrect lock ordering.

Understanding these mistakes is an important part of writing reliable multithreaded programs.

---

# What You Will Learn

In this chapter, you will learn:

- What Pthreads is
- Creating threads
- Thread functions
- Passing arguments
- Returning values
- Joining threads
- Detached threads
- Thread attributes
- Mutexes
- Condition variables
- Read-write locks
- Barriers
- Thread synchronization
- Common programming mistakes
- Best practices for writing multithreaded applications

---

# Why This Chapter Matters

Pthreads is one of the most widely used threading libraries in systems programming.

Understanding it is essential for:

- Linux development
- Operating systems
- High-performance computing
- Web servers
- Databases
- Embedded systems
- Game engines
- Cybersecurity tools

It provides the practical skills needed to transform the theory of concurrency into working multithreaded applications.

---

# Roadmap

```
Program
     │
     ▼
pthread_create()
     │
     ▼
Multiple Threads
     │
     ▼
Shared Resources
     │
     ▼
Synchronization
     │
     ▼
pthread_join()
     │
     ▼
Program Completion
```

By the end of this chapter, you will understand how to use the POSIX Threads library to build safe, efficient, and scalable multithreaded programs.

In the next chapter, we will apply everything learned so far to a real-world synchronization problem:

**The Dining Philosophers Problem**, one of the most famous concurrency problems in computer science.