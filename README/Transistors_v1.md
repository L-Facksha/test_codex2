# Chapter 2 — Transistors

> A computer cannot think with electricity alone.
>
> Electricity naturally wants to flow from one place to another, but a computer needs something much more powerful:
>
> It must decide **when** electricity is allowed to flow and **when** it must stop.
>
> That decision is made by the **transistor**.

---

# Table of Contents

1. Why We Need Transistors
2. Conductors, Insulators and Semiconductors
3. Why Silicon?
4. Crystal Structure
5. Doping
6. N-Type Silicon
7. P-Type Silicon
8. PN Junction
9. What is a Transistor?
10. MOSFET Structure
11. How a Transistor Works
12. Gate
13. Source
14. Drain
15. ON State
16. OFF State
17. Representing Binary
18. Billions of Transistors
19. Summary

---

# Why We Need Transistors

Imagine a simple wire.

```
Battery

+

│

│

────────────── Wire ──────────────

│

Bulb

│

-
```

As soon as the battery is connected,

electrons begin moving.

```
Electron →

Electron →

Electron →

Electron →
```

The bulb lights up.

This is useful.

But computers must answer questions like:

- Should this instruction execute?
- Should this memory cell change?
- Should this value be added?
- Should this file be opened?
- Should this webpage load?

A simple wire cannot answer these questions.

A wire only knows one thing:

> If electricity is available,
> I will let it pass.

A computer needs something smarter.

It needs a component capable of saying

```
Stop.
```

or

```
Go.
```

millions or even billions of times every second.

That component is called a **transistor**.

---

# The Problem with Copper

Copper is an excellent conductor.

```
Copper

████████████████████
```

When voltage is applied,

electrons immediately begin moving.

```
Battery

↓

Copper Wire

↓

Current flows
```

No matter what we do,

copper keeps conducting electricity.

That is exactly what we want for electrical wiring.

It is **not** what we want for building a computer.

A processor must constantly switch electricity ON and OFF.

Copper cannot do that.

---

# The Problem with Plastic

Now consider plastic.

```
Plastic

□□□□□□□□□□□□□□
```

Plastic refuses to conduct electricity.

Even if we apply voltage,

almost no current flows.

```
Battery

↓

Plastic

↓

Nothing happens
```

Plastic is therefore useless as a switch.

It is always OFF.

---

# We Need Something Between Them

Imagine three doors.

Door 1

```
Always Open
```

Door 2

```
Always Closed
```

Door 3

```
Can Open

or

Can Close
```

Which one would you choose to build a computer?

Obviously,

Door 3.

That is exactly what engineers searched for.

A material that could behave like

```
Conductor

↓

or

↓

Insulator
```

depending on the situation.

That material is called a

**Semiconductor.**

---

# Conductors

Conductors allow electrons to move easily.

Examples include

- Copper
- Gold
- Silver
- Aluminum

Inside a conductor,

many electrons are free to move.

```
Electron →

Electron →

Electron →

Electron →
```

Very little energy is required.

That is why electrical cables are made from copper.

---

# Insulators

Insulators prevent electrons from moving.

Examples include

- Plastic
- Rubber
- Glass
- Ceramic

Inside an insulator,

electrons are tightly bound to their atoms.

```
Electron

✖

Cannot move
```

That is why touching the plastic around an electrical wire is safe.

The copper carries electricity.

The plastic blocks it.

---

# Semiconductors

Semiconductors are unique.

Sometimes they behave like conductors.

Sometimes they behave like insulators.

Their behavior depends on

- Voltage
- Temperature
- Light
- Chemical composition

Unlike copper,

their conductivity can be controlled.

That single property changed human history.

Without semiconductors,

there would be

- No CPUs
- No RAM
- No SSDs
- No smartphones
- No Internet
- No Artificial Intelligence

Every modern electronic device depends on semiconductors.

---

# Why Silicon?

Many people think silicon is used because it conducts electricity well.

That is **not true.**

Copper conducts electricity much better than silicon.

Then why don't we build CPUs from copper?

Because copper has one fatal problem.

```
Copper

Always conducts.
```

A processor does not want electricity to flow all the time.

It wants electricity to flow only when instructed.

Silicon makes this possible.

---

## Silicon's Position

Silicon is the

```
14th element
```

of the periodic table.

It belongs to Group 14.

This means

each silicon atom has

```
4

valence electrons.
```

Those four electrons are the reason silicon is so special.

They allow silicon to create a perfectly balanced crystal structure while still being easy to manipulate through doping.

Later,

we'll see why changing only one atom out of millions completely changes how electricity moves.

---

# Why Not Gold?

Gold is an excellent conductor.

```
Very low resistance.
```

However,

it suffers from the same problem as copper.

```
Electricity

↓

Always flows
```

Gold cannot become an electronic switch.

It is useful for electrical connections,

but useless for making transistors.

---

# Why Not Plastic?

Plastic never conducts.

```
Always OFF
```

A processor needs billions of switches,

not billions of permanently closed doors.

---

# Silicon Solves Both Problems

Silicon sits exactly between

```
Copper

Always ON
```

and

```
Plastic

Always OFF
```

Silicon can become

```
ON

↓

OFF

↓

ON

↓

OFF
```

simply by changing the electrical conditions around it.

That ability makes silicon the perfect material for building transistors.

---

# Why Silicon Became the Industry Standard

Silicon has several advantages.

## It is abundant.

Silicon is one of the most common elements on Earth.

Most of it comes from sand.

```
Sand

↓

Silicon

↓

Computer Chips
```

---

## It is inexpensive.

Because silicon is abundant,

it is much cheaper than precious metals.

---

## It is stable.

Silicon remains reliable even after billions of switching operations.

Modern processors can operate continuously for years.

---

## It forms silicon dioxide.

When silicon touches oxygen,

it naturally creates

```
Silicon Dioxide

SiO₂
```

This forms an extremely thin insulating layer.

That natural insulation is one of the reasons MOSFET transistors became practical.

Very few other semiconductor materials can do this as effectively.

---

At this point we understand **why silicon was chosen**.

The next step is understanding **how silicon atoms arrange themselves** and why that arrangement determines whether electricity can move.

The next section begins with **Crystal Structure**.