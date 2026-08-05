# Operating System

> Hardware alone cannot run applications.
> An Operating System (OS) acts as the bridge between users, software, and hardware, managing every resource inside a computer and allowing multiple programs to run efficiently and safely.

---

# Introduction

In the previous chapters, we studied how computers are built from the ground up.

We started with electricity, learned how transistors become logic gates, how logic gates build CPUs, and how memory stores information.

At this point, we have all the hardware necessary to build a computer.

However, a question remains:

> **How does a program actually communicate with the hardware?**

A program cannot directly control the CPU, RAM, keyboard, disk, or network card.

If every application accessed hardware directly, the computer would quickly become unstable:

- Two programs could write to the same memory.
- Multiple applications could control the same device simultaneously.
- Any program could crash the entire machine.
- Security would be impossible.

To solve these problems, computers use an **Operating System (OS).**

The operating system is the software responsible for managing every hardware resource and providing a safe environment where applications can execute.

Without an operating system, modern computers would be extremely difficult to use.

---

# What is an Operating System?

An Operating System is system software that manages the interaction between:

- Hardware
- Applications
- Users

Instead of allowing programs to communicate directly with hardware, every request passes through the operating system.

```
User
   │
   ▼
Application
   │
   ▼
Operating System
   │
   ▼
Hardware
```

The operating system controls access to every hardware component and ensures that all running programs can safely share system resources.

---

# Responsibilities of an Operating System

An operating system performs many critical tasks, including:

- Process management
- Thread management
- Memory management
- File system management
- Device management
- CPU scheduling
- User management
- Security and permissions
- Networking
- Input/Output management
- Interrupt handling

These responsibilities allow applications to focus on solving problems rather than controlling hardware directly.

---

# Why Do We Need an Operating System?

Imagine writing a simple program that prints:

```c
printf("Hello, World!\n");
```

Without an operating system, your program would need to know:

- How to communicate with the monitor.
- How to access video memory.
- How to control the graphics hardware.
- How to initialize the display.

Instead, the operating system provides simple interfaces that hide this complexity.

Applications request services, and the operating system performs the hardware-specific work.

---

# What You Will Learn

In this chapter, you will learn:

- What an Operating System is
- Why Operating Systems exist
- System Software vs Application Software
- User Space
- Kernel Space
- Hardware Abstraction
- Process Management
- Thread Management
- Memory Management
- CPU Scheduling
- File Systems
- Device Drivers
- Interrupt Handling
- System Calls
- Boot Process
- Linux Architecture
- Types of Operating Systems
- Security and Permissions

---

# Types of Operating Systems

Operating systems can be designed for different purposes.

Examples include:

- Desktop Operating Systems
- Server Operating Systems
- Mobile Operating Systems
- Embedded Operating Systems
- Real-Time Operating Systems (RTOS)

Common examples include:

- Linux
- Windows
- macOS
- Android
- iOS

Although they differ internally, they all perform the same fundamental responsibilities.

---

# Why This Chapter Matters

Understanding operating systems explains many concepts encountered during software development, including:

- Why programs become processes.
- Why applications cannot access hardware directly.
- How multiple programs run simultaneously.
- Why permissions exist.
- Why memory is protected.
- How files are stored.
- How multitasking works.

Whether you're developing applications, studying cybersecurity, building embedded systems, or writing kernels, a solid understanding of operating systems is essential.

---

# Roadmap

This chapter follows the flow of a request through the computer.

```
User
    │
    ▼
Application
    │
    ▼
Operating System
    │
    ▼
Kernel
    │
    ▼
Hardware
```

In the next chapter, we will dive deeper into the most important part of every operating system:

**The Kernel.**

We will explore how it communicates directly with hardware, manages resources, handles system calls, and makes modern computing possible.