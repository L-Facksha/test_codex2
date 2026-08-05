# Chapter 1 — Electricity

> Everything a computer does begins with electricity. Before understanding transistors, CPUs, memory, or operating systems, we must first understand what electricity is and how it behaves.

---

# Table of Contents

- What is Electricity?
- Matter
- Atoms
- Electric Charge
- Static Electricity
- Electric Current
- Conventional Current vs Electron Flow
- Conductors
- Insulators
- Voltage
- Resistance
- Ohm's Law
- Electrical Power
- Alternating Current (AC)
- Direct Current (DC)
- Batteries
- Power Supply Unit (PSU)
- How Electricity Reaches the CPU
- Summary

---

# What is Electricity?

Electricity is the movement of electric charges.

In modern electronics, these charges are almost always **electrons**.

Imagine millions of tiny particles moving through a wire.

```
Electron → Electron → Electron → Electron
```

As these electrons move, they carry energy from one place to another.

That movement is called **electric current**.

Without electricity:

- Computers cannot execute instructions.
- CPUs cannot perform calculations.
- RAM cannot store temporary information.
- SSDs cannot read or write data.

Every operation inside a computer depends on controlling electricity.

---

# Matter

Everything around us is made of **matter**.

Examples include:

- Air
- Water
- Plastic
- Copper
- Iron
- Wood
- Human bodies

Matter is made of extremely small building blocks called **atoms**.

```
Everything

↓

Matter

↓

Atoms
```

---

# Atoms

An atom is the basic unit of matter.

It consists of three types of particles.

```
             Electron (-)

                  ●


Electron ●     Nucleus     ● Electron

                  ●

             Electron (-)
```

Inside the nucleus are:

- Protons
- Neutrons

Outside the nucleus are:

- Electrons

---

## Proton

A proton has a **positive charge (+)**.

Protons remain inside the nucleus and normally do not move.

---

## Neutron

A neutron has **no electric charge**.

It helps stabilize the nucleus.

---

## Electron

An electron has a **negative charge (-)**.

Unlike protons, electrons can move between atoms.

This movement creates electricity.

---

# Electric Charge

Every charged particle has one of two charges.

Positive

```
+
```

Negative

```
-
```

Basic rules:

```
Positive + Positive

↓

Repel

-----------------------

Negative + Negative

↓

Repel

-----------------------

Positive + Negative

↓

Attract
```

These interactions are responsible for electrical behavior.

---

# Static Electricity

Sometimes electrons move from one object to another but remain trapped.

Example:

Rubbing a balloon on your hair.

```
Hair

↓

Electrons move

↓

Balloon
```

The balloon becomes electrically charged.

This is called **static electricity** because the charges remain in one place instead of continuously flowing.

---

# Electric Current

Electric current is the continuous movement of electrons through a conductor.

```
Wire

Electron →

Electron →

Electron →

Electron →
```

Current tells us **how much electric charge moves every second**.

The SI unit of current is the **Ampere (A)**.

Symbol:

```
I
```

The larger the current, the more electrons are moving.

---

# Conventional Current vs Electron Flow

This often confuses beginners.

Electrons actually move like this:

```
Negative

──────────────▶

Positive
```

However, before electrons were discovered, scientists assumed current flowed the opposite way.

That convention is still used today.

Conventional current:

```
Positive

──────────────▶

Negative
```

Electron flow:

```
Negative

──────────────▶

Positive
```

Modern electronics uses both conventions depending on the context.

---

# Conductors

A conductor allows electrons to move easily.

Examples:

- Copper
- Silver
- Gold
- Aluminum

Inside a conductor:

```
Electron →

Electron →

Electron →

Electron →
```

Copper is the most common material used in electrical wiring because it conducts electricity efficiently and is relatively inexpensive.

---

# Insulators

An insulator prevents electrons from moving easily.

Examples:

- Plastic
- Rubber
- Glass
- Dry wood

Inside an insulator:

```
Electron

✖

Cannot move freely
```

Electrical cables are coated with plastic because:

- Copper carries electricity.
- Plastic protects people from electric shock.

---

# Voltage

Voltage is the electrical force that pushes electrons.

Imagine a water tank.

```
Water Tank

██████████

Pressure

↓

Pipe
```

The pressure pushes water through the pipe.

Electricity works similarly.

Voltage pushes electrons through a conductor.

The SI unit of voltage is the **Volt (V)**.

Higher voltage means a stronger push.

---

# Resistance

Resistance opposes the movement of electrons.

Imagine two pipes.

Wide pipe:

```
████████████
```

Water flows easily.

Narrow pipe:

```
██
```

Water has difficulty flowing.

Electric current behaves the same way.

Higher resistance means:

- Less current.
- More difficulty for electrons to move.

Resistance is measured in **Ohms (Ω)**.

---

# Ohm's Law

Voltage, current, and resistance are directly related.

The relationship is:

```
Current = Voltage / Resistance
```

This means:

- Increasing voltage increases current.
- Increasing resistance decreases current.

Example:

Voltage:

```
12 V
```

Resistance:

```
6 Ω
```

Current:

```
2 A
```

Ohm's Law is one of the most fundamental laws in electrical engineering.

---

# Electrical Power

Power measures how much electrical energy is consumed or produced every second.

The SI unit is the **Watt (W)**.

Examples:

Phone charger

```
20 W
```

Laptop charger

```
65 W
```

Desktop computer

```
400–800 W
```

Power tells us how much work electricity is performing.

---

# Alternating Current (AC)

Alternating Current constantly changes direction.

```
← → ← → ← →
```

Examples:

- Wall outlets
- Home electricity
- Office electricity

Power companies distribute electricity using AC because it can travel long distances efficiently.

---

# Direct Current (DC)

Direct Current flows in only one direction.

```
→ → → → →
```

Examples:

- Batteries
- Smartphones
- Laptops
- Motherboards
- CPUs

Computers operate internally using DC.

---

# Batteries

A battery stores chemical energy and converts it into electrical energy.

Inside a battery:

```
Chemical Energy

↓

Electrical Energy

↓

Electrons flow
```

Batteries produce Direct Current (DC).

---

# Power Supply Unit (PSU)

Desktop computers receive Alternating Current (AC) from the wall outlet.

However, computer components require Direct Current (DC).

The Power Supply Unit converts:

```
Wall Outlet

↓

AC

↓

Power Supply Unit

↓

DC

↓

Motherboard

↓

CPU
```

Without the PSU, the CPU and other components could not operate safely.

---

# How Electricity Reaches the CPU

The path electricity follows inside a desktop computer is:

```
Wall Outlet

↓

Power Cable

↓

Power Supply Unit

↓

Motherboard

↓

Voltage Regulation Modules (VRMs)

↓

CPU
```

The VRMs carefully reduce and stabilize the voltage before delivering it to the processor.

Modern CPUs typically operate at around **1 volt**, despite the wall outlet providing much higher voltages.

---

# Summary

Electricity is the movement of electrons through conductive materials.

The important concepts introduced in this chapter are:

- Matter is made of atoms.
- Electrons are responsible for electricity.
- Conductors allow electrons to move.
- Insulators prevent electron movement.
- Voltage pushes electrons.
- Current measures electron flow.
- Resistance opposes electron flow.
- Ohm's Law relates voltage, current, and resistance.
- Computers operate using Direct Current (DC).
- The Power Supply Unit converts household AC into DC before powering computer components.

Electricity alone cannot perform computations.

To build computers, we need a way to control the flow of electricity.

The next chapter introduces **transistors**, tiny electronic switches that can turn electricity on and off billions of times per second. Every modern processor is built from billions of these switches.