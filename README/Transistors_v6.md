---

# Representing Binary

Now we finally arrive at one of the most important ideas in computer science.

A transistor has only **two stable states**.

It is either

```
ON
```

or

```
OFF
```

There is no stable "half-on" state used in digital electronics.

Engineers realized this perfectly matches mathematics.

Instead of writing

```
ON
```

they simply write

```
1
```

Instead of writing

```
OFF
```

they write

```
0
```

So every transistor naturally represents one binary value.

```
OFF

↓

0
```

```
ON

↓

1
```

This is why computers use binary.

It is **not because computers like the numbers 0 and 1.**

It is because transistors naturally behave like two-state switches.

---

# Why Not Decimal?

Humans use decimal.

```
0 1 2 3 4 5 6 7 8 9
```

Why doesn't a CPU use decimal too?

Imagine trying to build a transistor with ten stable electrical states.

```
0

1

2

3

4

5

6

7

8

9
```

Every voltage would have to be perfectly distinguished.

Even tiny electrical noise could change

```
4

↓

5
```

causing errors.

Instead,

a transistor only needs two voltage ranges.

```
Low Voltage

↓

0

-----------------------

High Voltage

↓

1
```

This makes digital electronics extremely reliable.

---

# Voltage Does Not Equal Binary

A beginner often thinks

```
0 Volts

↓

Binary 0

-------------------

1 Volt

↓

Binary 1
```

This is **not exactly true.**

Different processors use different voltages.

For example,

```
0.0 V

↓

OFF

----------------

0.8 V

↓

Maybe

----------------

1.1 V

↓

ON
```

The exact values change depending on the technology.

The important idea is simply

```
Low

↓

0

-------------------

High

↓

1
```

---

# One Transistor is Not Enough

A single transistor can only answer one question.

```
Current?

↓

Yes

or

No
```

That isn't enough to perform addition.

Or multiplication.

Or execute programs.

Instead,

engineers connect many transistors together.

```
T

↓

T

↓

T

↓

T
```

The result is something called

```
Logic Gates.
```

---

# Billions of Transistors

Modern processors contain an unbelievable number of transistors.

Older CPUs

```
Thousands
```

Modern CPUs

```
Billions
```

Example

```
10,000,000,000+

Transistors
```

Imagine one transistor switching.

```
0

↓

1

↓

0

↓

1
```

Now imagine

```
10 Billion
```

doing that simultaneously,

billions of times every second.

That is what a CPU does.

---

# How Fast Do They Switch?

Modern desktop processors often operate around

```
4 GHz

to

6 GHz
```

One gigahertz means

```
1 Billion

Operations

Per Second
```

So a processor running at

```
5 GHz
```

can perform approximately

```
5 Billion

Clock Cycles

Every Second.
```

Each clock cycle,

billions of transistors may change state.

---

# Everything Starts With One Switch

Let's follow the chain.

```
Electricity

↓

Transistor

↓

Logic Gate

↓

Adder

↓

Register

↓

ALU

↓

CPU

↓

Computer

↓

Operating System

↓

Applications

↓

Games

↓

Artificial Intelligence
```

Everything inside a computer starts from one tiny electronic switch.

---

# Real Example

Suppose the CPU wants to add

```
2 + 3
```

Humans write

```
2

+

3
```

The CPU never sees these numbers.

Instead,

everything becomes binary.

```
2

↓

0010

----------------

3

↓

0011
```

Those binary digits travel through millions of transistors.

Each transistor simply asks

```
Current?

↓

Yes

or

No
```

Individually,

they know almost nothing.

Together,

they perform arithmetic.

---

# Why Is This Amazing?

Imagine asking a single person to build a city.

Impossible.

Now imagine millions of people,

each performing one tiny task.

One lays bricks.

One paints walls.

One installs windows.

Eventually,

an entire city appears.

A CPU works the same way.

One transistor does almost nothing.

Billions working together create

- Mathematics
- Graphics
- Operating Systems
- Networking
- Artificial Intelligence
- Video Games
- Everything digital

---

# Chapter Summary

In this chapter we learned that electricity alone cannot build a computer.

Electricity must be controlled.

A transistor is an electronic switch that controls the flow of electricity.

To build a transistor, engineers use silicon because it is a semiconductor.

Silicon atoms naturally form a crystal lattice through covalent bonds.

Pure silicon conducts very poorly.

By adding tiny amounts of impurities—a process called **doping**—we create:

- **N-Type silicon**, which contains many free electrons.
- **P-Type silicon**, which contains many holes.

Joining these materials forms a **PN Junction**, where electrons and holes recombine to create a depletion region and an internal electric field.

The most widely used transistor today is the **MOSFET**.

A MOSFET has three main terminals:

- **Source** – provides electrons.
- **Drain** – receives electrons.
- **Gate** – controls the transistor using an electric field.

Applying voltage to the Gate creates a conductive channel between the Source and Drain.

Removing that voltage destroys the channel.

The transistor therefore has two stable states:

```
Current flows

↓

ON

↓

1
```

```
Current blocked

↓

OFF

↓

0
```

These binary states are the foundation of digital electronics.

One transistor is simple.

Billions of transistors connected together create logic gates.

Logic gates create arithmetic circuits.

Arithmetic circuits create processors.

Processors create computers.

Everything inside modern computing ultimately begins with a tiny switch made from silicon.

---

# Next Chapter

The next chapter is

```
03-Logic-Gates.md
```

There we will learn:

- How transistors are connected together.
- Why one transistor cannot perform calculations.
- How AND, OR, NOT, NAND, NOR and XOR gates work.
- Why NAND gates alone can build an entire CPU.
- How logic gates become adders, multiplexers, registers and finally a processor.