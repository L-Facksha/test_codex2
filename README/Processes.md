# Processes

> A program stored on disk is only a collection of instructions.
> It becomes useful only when the operating system loads it into memory and starts executing it.
>
> A running program is called a **process**.

---

# Introduction

In the previous chapter, we learned that the **kernel** is responsible for managing every resource inside the computer.

One of its most important responsibilities is managing **processes**.

When you double-click a program, type a command into the terminal, or launch a web browser, the operating system creates a new process.

Every application running on your computer is a process.

For example:

- Your web browser
- Your code editor
- Your music player
- Your terminal
- Your game

Each of these applications runs independently as its own process.

The operating system keeps them isolated so that one process cannot accidentally modify or crash another.

Without processes, modern multitasking would not exist.

---

# What is a Process?

A **process** is a program that is currently being executed.

Think of it this way:

```
Program on Disk

        │

        ▼

Loaded into RAM

        │

        ▼

Executing

        │

        ▼

Process
```

A program is simply a file.

A process is that file while it is running.

---

# Program vs Process

These two terms are often confused.

A **program** is passive.

It sits on the storage device waiting to be executed.

A **process** is active.

It is executing instructions on the CPU.

Example:

```
/usr/bin/firefox
```

This file is a program.

When you launch Firefox,

Linux creates

```
Process #2510
```

Now Firefox is a running process.

You can even open Firefox twice.

```
Firefox Window #1

↓

Process 2510

-------------------

Firefox Window #2

↓

Process 2714
```

Same program.

Different processes.

---

# What Does a Process Contain?

A process is much more than executable code.

It contains:

- Program instructions
- Variables
- Stack
- Heap
- Open files
- CPU registers
- Program Counter
- Environment variables
- Process ID
- Scheduling information

Each process has its own isolated resources.

---

# Process Memory Layout

Every process has its own memory space.

```
+----------------------+
|      Stack           |
+----------------------+
|                      |
|       Heap           |
+----------------------+
|   Global Variables   |
+----------------------+
|     Program Code     |
+----------------------+
```

The kernel protects this memory from other processes.

One process cannot directly read another process's memory.

---

# Process Identifier (PID)

Every process has a unique identifier.

This number is called the **PID** (Process ID).

Example:

```
PID = 1023
```

Linux uses PIDs to identify running processes.

Commands like

```bash
ps
top
htop
kill
```

all use the PID.

---

# Parent and Child Processes

Processes can create new processes.

When one process creates another:

```
Parent Process

        │

        ▼

Child Process
```

The parent continues running.

The child becomes an independent process.

On Linux,

this is usually done using:

```c
fork();
```

---

# Process States

A process is not always executing.

It moves through several states.

```
New

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

The scheduler moves processes between these states.

---

# Multitasking

Modern operating systems run many processes at once.

For example:

```
Browser

Editor

Terminal

Music Player

Game
```

Even if your computer has only one CPU core,

it rapidly switches between processes.

This creates the illusion that everything runs simultaneously.

---

# Context Switching

Only one process can execute on a CPU core at a time.

When switching processes,

the kernel saves the current process state.

```
Process A

↓

Save CPU Registers

↓

Load Process B Registers

↓

Run Process B
```

This operation is called a **Context Switch**.

---

# Process Scheduling

The kernel decides

- which process runs,
- when it runs,
- and for how long.

This decision is made by the **CPU Scheduler**.

The scheduler attempts to balance:

- Performance
- Fairness
- Responsiveness

---

# Creating Processes

Processes are commonly created using:

```c
fork();
```

After calling `fork()`,

there are two nearly identical processes.

```
Parent

↓

fork()

↓

Parent

Child
```

The child receives its own PID.

---

# Executing Another Program

Sometimes a process wants to replace itself with another program.

Linux provides:

```c
exec()
```

The current process remains,

but its program is replaced.

---

# Waiting for Child Processes

A parent process can wait until one of its children finishes.

This is done using:

```c
wait();
```

or

```c
waitpid();
```

---

# Process Communication

Processes are isolated.

Sometimes they need to exchange information.

Common communication methods include:

- Pipes
- Signals
- Shared Memory
- Message Queues
- Sockets

These mechanisms are collectively called **Inter-Process Communication (IPC).**

---

# What You Will Learn

In this chapter, you will learn:

- What a process is
- Program vs Process
- Process memory layout
- Process ID (PID)
- Parent and child processes
- Process states
- CPU scheduling
- Context switching
- fork()
- exec()
- wait()
- Zombie processes
- Orphan processes
- Process communication (IPC)

---

# Why This Chapter Matters

Understanding processes explains:

- How applications start.
- Why every program has a PID.
- How Linux runs multiple programs simultaneously.
- Why programs are isolated.
- How the operating system schedules work.
- How parent and child processes communicate.

Processes are one of the most fundamental concepts in operating systems.

Understanding them is essential before learning about **threads**, which allow multiple execution paths inside a single process.

---

# Roadmap

```
Program
     │
     ▼
Process
     │
     ▼
Scheduler
     │
     ▼
CPU
     │
     ▼
Execution
     │
     ▼
Termination
```

By the end of this chapter, you will understand how a program becomes a running process and how the operating system manages thousands of processes simultaneously.

In the next chapter, we will study **Threads**, which allow multiple execution paths to exist inside a single process.