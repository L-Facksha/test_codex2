# Logic Gates

> Logic gates are the fundamental building blocks of every digital computer.
> They take one or more binary inputs (`0` or `1`), apply a logical rule, and produce a single binary output.
>
> Every calculation, comparison, memory access, and decision performed by a CPU ultimately relies on logic gates.

---

# Introduction

In the previous chapter, we learned that a **transistor** is an electronic switch.

A transistor has only two stable states:

```
OFF → 0

ON  → 1
```

A single transistor can only control whether electricity flows or not.

Although this is enough to represent a binary value, it is **not enough to perform computation**.

For example, a computer constantly needs to answer questions such as:

- Should this instruction execute?
- Are these two numbers equal?
- Should this value be stored in memory?
- Is the user allowed to access this file?
- Should the processor jump to another instruction?

These decisions require combining several binary values together according to specific rules.

This is exactly what **Logic Gates** do.

---

# What is a Logic Gate?

A **Logic Gate** is an electronic circuit built from one or more transistors.

It receives one or more binary inputs, applies a predefined logical rule, and produces a binary output.

```
      Input A

          │

          ▼

     ┌───────────┐
     │ Logic Gate│
     └───────────┘
          ▲
          │

      Input B

          │

          ▼

        Output
```

Unlike software, a logic gate does not make decisions by "thinking."

It simply follows a fixed electrical rule.

Whenever the inputs change, the output changes immediately according to that rule.

---

# Binary Inputs and Outputs

Logic gates only understand two values.

```
0 → FALSE → OFF

1 → TRUE  → ON
```

Every input entering a logic gate is one of these two values.

The output is also one of these two values.

For example,

```
Input A = 1

Input B = 0
```

After applying the gate's rule, the output might become

```
Output = 0
```

or

```
Output = 1
```

depending on the type of logic gate.

---

# How Logic Gates Work

Imagine two switches connected to an electronic circuit.

Each switch can be either ON or OFF.

```
Switch A

ON

↓

1

----------------

Switch B

OFF

↓

0
```

The logic gate reads both inputs.

Then it applies its internal rule.

For example,

an **AND gate** asks:

> "Are **both** inputs equal to 1?"

If the answer is yes,

the output becomes

```
1
```

Otherwise,

the output becomes

```
0
```

Every type of logic gate follows a different logical rule, but they all operate using the same principle:

```
Read Inputs

↓

Apply Rule

↓

Produce Output
```

---

# Truth Tables

The behavior of every logic gate is described using a **Truth Table**.

A truth table lists every possible combination of inputs and the corresponding output.

Example:

| A | B | Output |
|---|---|--------|
| 0 | 0 | ? |
| 0 | 1 | ? |
| 1 | 0 | ? |
| 1 | 1 | ? |

Each logic gate fills this table differently.

Learning these tables is essential because they define exactly how digital circuits behave.

---

# Types of Logic Gates

Modern digital systems are primarily built using six basic logic gates.

## NOT

Reverses the input.

```
0 → 1

1 → 0
```

---

## AND

Produces `1` only if **every input** is `1`.

---

## OR

Produces `1` if **at least one input** is `1`.

---

## XOR (Exclusive OR)

Produces `1` only if the inputs are different.

---

## NAND

The opposite of an AND gate.

Produces `0` only when every input is `1`.

---

## NOR

The opposite of an OR gate.

Produces `1` only when every input is `0`.

---

# Why Logic Gates Are Important

One logic gate performs only one very simple operation.

However,

modern processors contain **billions of transistors** organized into **millions of logic gates**.

These gates work together to perform:

- Arithmetic operations
- Comparisons
- Memory access
- Data movement
- Instruction decoding
- Program execution

Every application running on your computer is ultimately executed through logic gates switching between `0` and `1` billions of times every second.

---

# Building Larger Circuits

A single logic gate cannot build a computer.

Instead, logic gates are connected together to create increasingly complex digital circuits.

```
Transistor
      │
      ▼
Logic Gate
      │
      ▼
Half Adder
      │
      ▼
Full Adder
      │
      ▼
Arithmetic Logic Unit (ALU)
      │
      ▼
Registers
      │
      ▼
Control Unit
      │
      ▼
CPU
```

Every level is built from the level below it.

---

# What You Will Learn

In this chapter, we will study:

- Boolean Logic
- Binary Inputs and Outputs
- Truth Tables
- NOT Gate
- AND Gate
- OR Gate
- XOR Gate
- NAND Gate
- NOR Gate
- Building Logic Gates with Transistors
- Universal Gates
- Combining Gates into Larger Circuits
- How Logic Gates Become the Foundation of Modern CPUs

By the end of this chapter, you will understand how a collection of simple electronic circuits can perform every logical operation required by a modern computer.