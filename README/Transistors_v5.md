---

# What is a MOSFET?

So far we have learned:

- Electricity
- Silicon
- Doping
- N-Type Silicon
- P-Type Silicon
- PN Junction

These concepts are enough to build a **diode**.

A diode is useful because it lets current flow in only one direction.

```
Current →

✔

Current ←

✖
```

However...

A computer needs much more than a diode.

A CPU must decide

- Execute?
- Don't execute?

Read memory?

Don't read memory?

Store data?

Ignore data?

A diode cannot make decisions.

It simply follows physics.

We need something that can be **controlled**.

That device is called a

# MOSFET

MOSFET stands for

```
Metal

Oxide

Semiconductor

Field

Effect

Transistor
```

Don't worry about memorizing the name.

Instead,

understand **what it does**.

A MOSFET is simply

> An electronic switch controlled by voltage.

---

# Why CPUs Use MOSFETs

Imagine a normal light switch.

```
OFF

↓

ON

↓

OFF

↓

ON
```

Every time you touch it,

it moves.

A CPU cannot use mechanical switches.

Why?

Because modern CPUs switch

```
5,000,000,000+

times

every second.
```

No mechanical object can move that fast.

Instead,

a MOSFET switches using **electric fields**.

Nothing physically moves.

Only electrons move.

This makes switching almost unbelievably fast.

---

# A MOSFET Has Three Main Parts

A MOSFET contains

```
        Gate

          │

          │

Source ─────── Drain
```

Every transistor inside your processor has these three terminals.

Each one has a specific job.

---

# Source

Think of the Source as

```
A Water Tank
```

filled with water.

Except,

instead of water,

it contains

```
Electrons.
```

```
Source

● ● ● ● ● ● ●
```

The Source is where current begins.

---

# Drain

The Drain is where electrons want to go.

```
Source

↓

Drain
```

If a path exists,

electrons travel

```
Source

↓

Drain
```

If no path exists,

nothing moves.

---

# Gate

The Gate is the most interesting part.

Unlike the Source and Drain,

almost **no current enters the Gate.**

Instead,

the Gate acts like a

```
Remote Control.
```

Imagine a garage door.

```
Garage

↓

Door

↓

Remote Button
```

The remote does not push the door.

It simply tells the motor

```
Open.
```

The Gate behaves the same way.

It does not provide current.

It only tells the transistor

```
Allow current.

or

Stop current.
```

---

# The Oxide Layer

Something very important sits under the Gate.

```
Gate

██████████

Oxide

──────────

Silicon
```

The oxide layer is an insulator.

Usually,

it is made from

```
Silicon Dioxide

SiO₂
```

Remember Chapter 1.

Insulators stop current.

That means

```
Gate

↓

Current

↓

✖

Cannot pass
```

Almost no current flows into the Gate.

Instead,

the Gate creates something much more important.

---

# Electric Field

When voltage is applied to the Gate,

electrons cannot cross the oxide.

Instead,

an electric field appears.

Imagine a magnet.

```
Magnet

↓

Magnetic Field

↓

Objects move
```

The magnet never touches the object.

The field does the work.

Exactly the same thing happens here.

Voltage creates

```
Electric Field
```

The electric field reaches through the oxide

and changes the silicon underneath.

---

# Before Voltage

Initially,

there is no conductive path.

```
Gate

0 V

────────────

Source

✖

Drain
```

Electrons cannot move.

Current equals

```
0
```

The transistor is OFF.

---

# Applying Voltage

Now apply voltage.

```
Gate

1 V
```

The electric field attracts electrons toward the surface of the silicon.

```
Before

Electron

↓

Far Away

---------------

After

Electron

↓

Pulled Up
```

More and more electrons gather beneath the Gate.

Eventually,

they form something incredible.

---

# Conductive Channel

Enough electrons collect together

to create a thin conducting path.

```
Source

████████████

Drain
```

This path is called

```
The Channel.
```

The channel did not exist before.

The Gate created it.

---

# Current Begins Flowing

Now the electrons finally have a path.

```
Source

↓

Channel

↓

Drain
```

Current flows.

The transistor has become

```
ON
```

---

# Removing the Voltage

Now remove the voltage.

```
Gate

0 V
```

The electric field disappears.

Without the field,

the electrons spread back out.

```
Channel

↓

Disappears
```

Now the Source and Drain are disconnected again.

```
Source

✖

Drain
```

Current stops.

The transistor is OFF again.

---

# Why This Is Amazing

Notice something incredible.

Nothing moved.

No gears.

No springs.

No buttons.

No motors.

The transistor changed from

```
OFF

↓

ON

↓

OFF
```

using only an electric field.

This allows billions of transistors to switch billions of times every second.

---

# A Simple Analogy

Imagine two rooms.

```
Room A

██████

Door

██████

Room B
```

People want to move from Room A to Room B.

The Door represents

```
The Channel.
```

The Gate controls the Door.

```
Gate

↓

Open Door

↓

People Walk
```

Remove the command.

```
Gate

↓

Close Door

↓

Nobody Walks
```

The Source never changes.

The Drain never changes.

Only the Door changes.

That is exactly how a MOSFET works.

---

# Why Doesn't the Gate Consume Current?

This question confuses many beginners.

The answer is simple.

Remember the oxide layer.

```
Gate

↓

Oxide

↓

Silicon
```

The oxide is an insulator.

Current cannot pass through it.

Instead,

the Gate only creates an electric field.

This means

- Very little power is consumed.
- Very little heat is generated.
- Billions of transistors can operate simultaneously.

This is one of the main reasons modern processors use MOSFETs.

---

# Summary

A MOSFET is an electrically controlled switch.

It contains three main terminals:

- Source
- Gate
- Drain

The Source provides electrons.

The Drain receives electrons.

The Gate does not carry current.

Instead,

applying voltage to the Gate creates an electric field.

That electric field forms a conductive channel between the Source and Drain.

When the channel exists,

current flows.

When the channel disappears,

current stops.

The two stable states of the transistor are therefore

```
Current flows

↓

ON

↓

1

-------------------

Current stops

↓

OFF

↓

0
```

This simple idea—allowing or blocking current—is the foundation of every digital computer ever built.