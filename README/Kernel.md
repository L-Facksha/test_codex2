# Kernel

> The kernel is the core component of an operating system.
> It is the only software that communicates directly with the hardware and manages every resource inside the computer.
>
> Every application you run depends on the kernel, even if you never interact with it directly.

---

# Introduction

In the previous chapter, we learned that an **Operating System** acts as the bridge between applications and hardware.

Applications never communicate directly with the CPU, RAM, disk, keyboard, or network card.

Instead, every request passes through the operating system.

However, an operating system is made of many different components.

The most important of these components is the **Kernel**.

The kernel is the first part of the operating system loaded into memory during boot.

Once running, it remains in memory until the computer shuts down.

Unlike normal applications, the kernel executes with the highest level of privilege and has unrestricted access to every hardware component.

Without a kernel:

- Programs cannot execute safely.
- Memory cannot be protected.
- Hardware cannot be shared.
- Files cannot be accessed.
- Devices cannot communicate.

The kernel is responsible for making the entire computer function.

---

# What is a Kernel?

A **Kernel** is the central component of an operating system.

Its job is to manage hardware resources and provide services that applications need.

Every time a program wants to:

- Read a file
- Allocate memory
- Create a process
- Create a thread
- Send data over the network
- Access the keyboard
- Display something on the screen

it asks the kernel to perform the operation.

```
Application
      │
      ▼
 System Call
      │
      ▼
    Kernel
      │
      ▼
   Hardware
```

The application never talks directly to the hardware.

The kernel does.

---

# Responsibilities of the Kernel

The kernel manages nearly every hardware resource in the computer.

Its responsibilities include:

- Process management
- Thread management
- CPU scheduling
- Memory management
- Virtual memory
- Device management
- File system access
- Interrupt handling
- System calls
- Security and permissions
- Networking
- Hardware communication

The kernel coordinates these components to ensure that multiple programs can run safely and efficiently.

---

# Kernel Space vs User Space

Modern operating systems divide memory into two regions.

## User Space

Applications execute in **User Space**.

Programs running here have limited privileges.

They cannot:

- Access hardware directly
- Modify kernel memory
- Execute privileged CPU instructions

Instead, they must ask the kernel for help.

---

## Kernel Space

The kernel executes in **Kernel Space**.

Here it has complete control over the computer.

It can:

- Access all memory
- Control devices
- Schedule the CPU
- Handle interrupts
- Execute privileged instructions

This separation improves both security and system stability.

```
+---------------------------+
|        User Space         |
|---------------------------|
| Browser                   |
| Terminal                  |
| Game                      |
| Editor                    |
+---------------------------+

            │
      System Calls
            │

+---------------------------+
|       Kernel Space        |
|---------------------------|
| Scheduler                 |
| Memory Manager            |
| Drivers                   |
| File System               |
+---------------------------+

            │

        Hardware
```

---

# How Applications Communicate with the Kernel

Applications cannot execute privileged operations directly.

Instead, they perform **System Calls**.

Example:

When you write

```c
printf("Hello");
```

your program eventually performs a system call that asks the kernel to write data to the terminal.

Similarly,

```c
open("file.txt");
```

does not access the disk itself.

Instead,

the kernel receives the request, communicates with the storage device, and returns the result.

---

# Types of Kernels

Different operating systems use different kernel designs.

### Monolithic Kernel

Most services execute inside the kernel itself.

Advantages:

- Very fast
- Efficient communication

Disadvantages:

- Larger kernel
- Bugs may affect the whole system

Example:

- Linux

---

### Microkernel

Only essential services remain inside the kernel.

Other services execute in user space.

Advantages:

- Better isolation
- Easier maintenance

Disadvantages:

- More communication overhead

Examples:

- MINIX
- QNX

---

### Hybrid Kernel

Combines ideas from both monolithic and microkernels.

Examples:

- Windows NT
- macOS (XNU)

---

# What You Will Learn

In this chapter, you will learn:

- What a Kernel is
- Kernel Space vs User Space
- Kernel Responsibilities
- Process Management
- Thread Management
- CPU Scheduler
- Context Switching
- Memory Management
- Virtual Memory
- Interrupts
- Exceptions
- Device Drivers
- File Systems
- System Calls
- Kernel Modules
- Linux Kernel Architecture
- Boot Sequence
- Kernel Initialization

---

# Why This Chapter Matters

Understanding the kernel explains many topics encountered in systems programming:

- Why programs cannot access hardware directly.
- Why segmentation faults occur.
- How multitasking works.
- Why system calls exist.
- How Linux manages resources.
- Why drivers are necessary.
- How operating systems remain secure and stable.

Whether you're building applications, studying operating systems, writing device drivers, or working in cybersecurity, understanding the kernel is essential.

---

# Roadmap

This chapter follows the journey of a hardware request.

```
Application
      │
      ▼
System Call
      │
      ▼
Kernel
      │
      ├────────► Memory Manager
      │
      ├────────► Process Scheduler
      │
      ├────────► Device Drivers
      │
      ├────────► File System
      │
      ▼
Hardware
```

By the end of this chapter, you will understand how the kernel controls every resource inside the computer and why it is considered the heart of every operating system.

In the next chapter, we will study one of the kernel's most important responsibilities:

**Processes**—how programs become running applications and how the operating system manages them.