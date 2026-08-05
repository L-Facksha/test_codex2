# Synchronization

> Threads make programs faster by allowing multiple tasks to execute at the same time.
>
> However, when multiple threads share the same data, they can interfere with one another.
>
> **Synchronization** is the set of techniques used to coordinate threads and ensure that shared resources are accessed safely and correctly.

---

# Introduction

In the previous chapter, we learned that threads within the same process share resources such as:

- Global variables
- Heap memory
- Open files
- Shared objects

This sharing is one of the biggest advantages of threads because communication is very fast.

However, it also creates one of the biggest challenges in concurrent programming.

Imagine two threads trying to update the same variable:

```c
counter++;
```

If both threads execute this statement simultaneously, the final value of `counter` may be incorrect.

This problem is called a **Race Condition**.

Synchronization exists to prevent these kinds of problems.

It provides rules that ensure multiple threads cooperate instead of interfering with each other.

---

# Why Synchronization is Necessary

Suppose two threads are updating a shared bank account.

Initial balance:

```
100$
```

Thread A wants to deposit:

```
+50$
```

Thread B wants to withdraw:

```
-20$
```

The correct result should be:

```
130$
```

If both threads modify the balance at the same time without coordination, the final result might become:

```
80$

or

150$

or

130$
```

The outcome becomes unpredictable.

Synchronization guarantees that shared data remains consistent.

---

# Critical Section

A **Critical Section** is a part of the program where shared data is accessed.

Example:

```c
counter++;
```

Although this looks like one instruction, the CPU actually performs several operations:

```
Read counter

↓

Add 1

↓

Write counter
```

If another thread interrupts during these steps, the data may become corrupted.

Critical sections must therefore be protected.

---

# Race Condition

A **Race Condition** occurs when two or more threads access the same shared resource at the same time and at least one thread modifies it.

Example:

```c
counter++;
```

Two threads execute this simultaneously.

Both read:

```
counter = 5
```

Each computes:

```
6
```

Both write:

```
6
```

Expected:

```
7
```

Actual:

```
6
```

One increment was lost.

---

# Mutual Exclusion

To prevent race conditions, only one thread should access a critical section at a time.

This principle is called **Mutual Exclusion**.

```
Thread A

↓

Critical Section

↓

Leave

↓

Thread B enters
```

Only one thread may be inside the protected section at any given moment.

---

# Mutex

The most common synchronization primitive is the **Mutex** (Mutual Exclusion).

A mutex behaves like a lock.

```
Unlocked

↓

Thread locks it

↓

Other threads wait

↓

Thread unlocks it

↓

Next waiting thread continues
```

Typical usage:

```c
pthread_mutex_lock(&mutex);

/* Critical Section */

pthread_mutex_unlock(&mutex);
```

---

# Semaphore

A semaphore is similar to a mutex but allows multiple threads to enter simultaneously.

Instead of being simply:

```
Locked

Unlocked
```

it maintains a counter.

Example:

```
Semaphore = 3
```

Three threads may enter.

The fourth must wait.

Semaphores are commonly used to limit access to resources such as:

- Database connections
- Thread pools
- Printers
- Network connections

---

# Condition Variables

Sometimes a thread should wait until a specific event happens.

Example:

```
Producer

↓

Produces data

↓

Consumer wakes up
```

Instead of constantly checking whether data is available, the consumer sleeps until it receives a signal.

Condition variables make waiting efficient.

---

# Read-Write Locks

Sometimes many threads only need to read data.

Reading simultaneously is safe.

Writing is not.

A Read-Write Lock allows:

```
Many Readers

↓

One Writer
```

This improves performance when reads are much more common than writes.

---

# Atomic Operations

Some operations are so common that modern CPUs provide hardware instructions to execute them safely.

These are called **Atomic Operations**.

An atomic operation cannot be interrupted halfway.

Example:

```c
atomic_fetch_add(&counter, 1);
```

Unlike:

```c
counter++;
```

the CPU guarantees that the increment happens as one indivisible operation.

---

# Deadlock

A **Deadlock** occurs when two or more threads wait for each other forever.

Example:

```
Thread A

locks Mutex 1

↓

waits for Mutex 2

------------------------

Thread B

locks Mutex 2

↓

waits for Mutex 1
```

Neither thread can continue.

The program becomes permanently blocked.

---

# Starvation

A thread suffers **Starvation** when it waits indefinitely because other threads continuously receive access to a resource first.

Unlike deadlock, the program still runs, but one thread never gets a chance to execute.

---

# Livelock

A **Livelock** is similar to a deadlock.

However, instead of stopping completely, the threads continue changing state but make no useful progress.

They remain busy without completing their work.

---

# Synchronization Primitives

The most common synchronization mechanisms include:

- Mutex
- Semaphore
- Condition Variable
- Read-Write Lock
- Spinlock
- Atomic Operations
- Barriers

Each primitive is designed for a different synchronization problem.

---

# What You Will Learn

In this chapter, you will learn:

- Why synchronization exists
- Critical Sections
- Race Conditions
- Mutual Exclusion
- Mutexes
- Semaphores
- Condition Variables
- Read-Write Locks
- Atomic Operations
- Deadlocks
- Starvation
- Livelocks
- Lock Ordering
- Best Practices for Thread Safety

---

# Why This Chapter Matters

Synchronization is one of the most important topics in concurrent programming.

Without synchronization:

- Data becomes corrupted.
- Programs produce random results.
- Threads interfere with one another.
- Applications become unstable.

Understanding synchronization is essential for:

- Operating Systems
- Multithreaded Applications
- Game Engines
- Databases
- Web Servers
- Distributed Systems

---

# Roadmap

```
Threads
     │
     ▼
Shared Resource
     │
     ▼
Critical Section
     │
     ▼
Synchronization
     │
     ├────────► Mutex
     ├────────► Semaphore
     ├────────► Condition Variable
     ├────────► Read-Write Lock
     └────────► Atomic Operations
                │
                ▼
         Safe Concurrent Programs
```

By the end of this chapter, you will understand why synchronization is necessary, how race conditions occur, and how different synchronization primitives work together to build safe and reliable multithreaded applications.

In the next chapter, we will move from the concepts of synchronization to their practical implementation using the **POSIX Threads (Pthreads)** library in C.