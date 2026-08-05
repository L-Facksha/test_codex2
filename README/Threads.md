# Threads

> A process allows a program to run.
> A thread allows multiple tasks to run **inside the same process**.
>
> Threads are the foundation of modern concurrent programming and are used by almost every modern application.

---

# Introduction

In the previous chapter, we learned that a **process** is a running program.

Every application on your computer executes as a separate process, each with its own memory space and system resources.

While this isolation improves security and stability, it also comes with a cost.

Imagine a web browser.

While you're using it, it must:

- Render the webpage
- Download images
- Execute JavaScript
- Play videos
- Respond to mouse clicks
- Load new tabs

If the browser used only one execution path, every task would have to wait for the previous one to finish.

The application would quickly become slow and unresponsive.

To solve this problem, operating systems introduced **threads**.

Threads allow a single process to perform multiple tasks simultaneously.

---

# What is a Thread?

A **thread** is the smallest unit of execution managed by the operating system.

It represents a sequence of instructions that the CPU executes.

A process always contains **at least one thread**, called the **main thread**.

Additional threads can be created to perform other tasks concurrently.

```
Process

    │

    ├────────► Main Thread

    ├────────► Thread 2

    ├────────► Thread 3

    └────────► Thread 4
```

Each thread executes independently while sharing the same process resources.

---

# Process vs Thread

Although they are closely related, processes and threads are different.

### Process

A process has its own:

- Virtual memory
- Heap
- Global variables
- Open files
- Process ID (PID)

Processes are isolated from one another.

---

### Thread

Threads belong to a process.

Threads share:

- Heap
- Global variables
- Open files
- Address space

Each thread has its own:

- Stack
- Registers
- Program Counter (PC)
- Thread ID (TID)

```
Process

+--------------------------------------+

Heap

Global Variables

Open Files

Address Space

----------------------------------------

Thread 1 → Stack

Thread 2 → Stack

Thread 3 → Stack

+--------------------------------------+
```

---

# Why Threads Exist

Threads improve application performance.

Instead of performing tasks one after another:

```
Task A

↓

Task B

↓

Task C
```

the application can execute them simultaneously.

```
Task A

Task B

Task C
```

This improves responsiveness and better utilizes modern multi-core processors.

---

# Real-World Examples

Most modern applications use threads.

### Web Browser

Different threads may handle:

- Rendering pages
- Network communication
- JavaScript execution
- User interface

---

### Video Game

Different threads may handle:

- Physics
- Rendering
- Audio
- Artificial Intelligence
- Networking

---

### Music Player

One thread:

- Plays audio

Another thread:

- Updates the interface

Another thread:

- Reads files

The user experiences smooth playback because these tasks execute concurrently.

---

# Thread Lifecycle

A thread usually passes through several states.

```
Created

↓

Ready

↓

Running

↓

Waiting

↓

Running

↓

Terminated
```

The operating system scheduler moves threads between these states.

---

# Thread Scheduling

Just like processes,

threads compete for CPU time.

The scheduler decides:

- Which thread runs
- Which thread waits
- How long each thread executes

If the CPU has multiple cores,

different threads may execute simultaneously.

If there is only one core,

the scheduler rapidly switches between threads,

creating the illusion of parallel execution.

---

# Shared Resources

Threads share most of the process resources.

For example:

```
Global Variable

↓

Shared

```

```
Heap

↓

Shared

```

```
Open Files

↓

Shared

```

This makes communication between threads very fast.

However,

sharing data introduces new problems.

---

# Race Conditions

Suppose two threads execute:

```c
counter++;
```

at the same time.

Both threads attempt to modify the same variable.

Because the operation is not atomic,

the final result may be incorrect.

This problem is called a **Race Condition**.

Race conditions are one of the biggest challenges in concurrent programming.

---

# Thread Synchronization

To safely share data,

threads use synchronization mechanisms.

Examples include:

- Mutex
- Semaphore
- Read-Write Lock
- Condition Variable
- Atomic Operations

These tools prevent multiple threads from modifying shared data simultaneously.

The next chapter will study these mechanisms in detail.

---

# Creating Threads

In POSIX systems,

threads are commonly created using:

```c
pthread_create()
```

A thread may terminate by:

```c
return;

```

or

```c
pthread_exit();
```

Another thread can wait for it using:

```c
pthread_join();
```

These functions form the foundation of multithreaded programming in C.

---

# What You Will Learn

In this chapter, you will learn:

- What a thread is
- Process vs Thread
- Thread memory layout
- Shared resources
- Private resources
- Thread lifecycle
- Thread scheduling
- Multi-core execution
- Concurrency vs Parallelism
- Race conditions
- Why synchronization is necessary

---

# Why This Chapter Matters

Understanding threads explains:

- Why modern applications remain responsive.
- How multiple tasks execute simultaneously.
- Why race conditions occur.
- Why synchronization primitives such as mutexes are necessary.
- How operating systems efficiently utilize modern CPUs.

Threads are the foundation of concurrent programming and are used extensively in operating systems, servers, databases, game engines, and high-performance applications.

---

# Roadmap

```
Program
     │
     ▼
Process
     │
     ▼
Main Thread
     │
     ├────────► Thread A
     ├────────► Thread B
     ├────────► Thread C
     └────────► Thread D
                 │
                 ▼
           Shared Resources
                 │
                 ▼
         Synchronization
```

By the end of this chapter, you will understand how multiple execution paths can exist inside a single process and why thread synchronization is essential.

In the next chapter, we will study **Synchronization**, where we'll learn how mutexes, semaphores, condition variables, and atomic operations prevent threads from interfering with one another.