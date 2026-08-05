---

# PN Junction

We now have two different materials.

One contains many free electrons.

```
N-Type

Electron Electron Electron Electron
```

The other contains many holes.

```
P-Type

Hole Hole Hole Hole
```

The obvious question is:

> **What happens if we place them next to each other?**

Engineers discovered that something remarkable happens automatically.

No battery is needed.

No external voltage is required.

Nature immediately begins reorganizing the electrons.

This boundary between the two materials is called the

**PN Junction**.

---

# Before Contact

Before the materials touch,

they are completely independent.

```
P-Type                  N-Type

○ ○ ○ ○ ○          ● ● ● ● ●
(Holes)            (Electrons)
```

Nothing moves because the materials are separated.

Each remains electrically neutral.

---

# The Moment They Touch

Now imagine sliding them together.

```
P-Type | N-Type

○ ○ ○ | ● ● ●
```

The free electrons inside the N-Type material immediately notice something.

There are empty holes on the other side.

Electrons naturally move to fill those holes.

```
Electron →

↓

Hole
```

This happens without anyone forcing it.

Nature always tries to reach a lower-energy, more stable state.

---

# Electron-Hole Recombination

When an electron falls into a hole,

both disappear.

```
Before

Electron ●

Hole ○

↓

After

Complete Bond
```

This process is called

**Recombination.**

Nothing has been destroyed.

The electron simply became part of a covalent bond again.

---

# The First Few Electrons Move

Only the electrons close to the junction move.

Imagine the junction.

```
P-Type        N-Type

○ ○ ○ | ● ● ●

        ↑

First electrons cross
```

Those first electrons fill nearby holes.

Now something interesting happens.

---

# Fixed Ions Remain

When an electron leaves the N-Type material,

it leaves behind the phosphorus atom that donated it.

Originally,

that phosphorus atom was electrically neutral.

Now it has lost an electron.

It becomes a

**Positive Ion.**

```
Before

P + Electron

↓

Neutral

------------------

After

P

↓

Positive Ion (+)
```

Notice something important.

The phosphorus atom cannot move.

Only the electron moved.

The atom remains fixed inside the crystal.

---

The same thing happens on the P-Type side.

When a hole is filled,

the boron atom effectively gains an electron.

It becomes a

**Negative Ion.**

Again,

the atom cannot move.

---

# The Depletion Region

After many electrons move,

an interesting region forms around the junction.

```
P-Type

○ ○ ○

--------

- - - -

--------

● ● ●

N-Type
```

Near the center,

there are almost

- No free electrons
- No holes

Only fixed ions remain.

This region is called the

**Depletion Region.**

Why "depletion"?

Because the mobile charge carriers have been depleted.

---

# Inside the Depletion Region

Imagine zooming in.

```
P-Type

- - - -

|

Depletion Region

|

+ + + +

N-Type
```

Notice:

Left side

```
Negative ions
```

Right side

```
Positive ions
```

These ions cannot move.

But they do create something important.

---

# The Built-in Electric Field

Remember from Chapter 1.

Opposite charges attract.

```
+

↓

← Electric Field →

↓

-
```

Inside the depletion region,

the positive and negative ions create their own electric field.

This field points from the positive ions toward the negative ions.

```
P-Type

- - - -

← Electric Field

+ + + +

N-Type
```

This electric field acts like an invisible wall.

---

# Why Electrons Stop Moving

At first,

electrons crossed the junction freely.

But every electron that crosses makes the electric field stronger.

Eventually,

the field becomes strong enough to push new electrons back.

```
Electron →

↓

Electric Field

↓

Pushes Back
```

Now equilibrium is reached.

Electrons still want to cross,

but the electric field pushes them away.

The two effects balance each other.

Current stops.

---

# Equilibrium

At equilibrium,

the PN Junction looks like this.

```
P-Type

○ ○

-------

Depletion

-------

● ●

N-Type
```

No current flows.

Even though free electrons still exist,

the electric field prevents them from crossing.

This is one of the most beautiful examples of nature finding balance.

---

# Applying an External Voltage

Now imagine connecting a battery.

The result depends on how we connect it.

There are two possibilities.

1.

Forward Bias

2.

Reverse Bias

---

# Forward Bias

Connect the battery like this.

```
Battery

+

↓

P-Type

|

PN Junction

|

N-Type

↓

-
```

Notice something.

The battery pushes holes toward the junction.

It also pushes electrons toward the junction.

```
Electron →

← Hole
```

Both move toward the depletion region.

---

# What Happens?

The battery's electric field opposes the built-in electric field.

```
Battery

→

←

Built-in Field
```

The depletion region becomes thinner.

```
Before

████████

After

██
```

Eventually,

electrons cross the junction freely.

Current begins flowing.

The junction is now conducting.

---

# Reverse Bias

Now reverse the battery.

```
Battery

+

↓

N-Type

|

PN Junction

|

P-Type

↓

-
```

Now the battery pulls electrons away from the junction.

It also pulls holes away.

```
Electron ←

Hole →
```

The depletion region grows wider.

```
Before

████

After

██████████████
```

The electric field becomes even stronger.

Almost no current can pass.

---

# Summary of Biasing

Forward Bias

```
Battery helps current.

↓

Depletion region shrinks.

↓

Current flows.
```

Reverse Bias

```
Battery opposes current.

↓

Depletion region grows.

↓

Current stops.
```

---

# Why Is This Important?

Everything we've learned so far leads to one incredible conclusion.

A PN Junction can act like an electrically controlled valve.

```
Forward Bias

↓

Open

-------------------

Reverse Bias

↓

Closed
```

This behavior is the foundation of

- Diodes
- Bipolar Junction Transistors (BJTs)
- MOSFETs
- Integrated Circuits
- CPUs
- RAM
- SSD Controllers

Every modern processor contains billions of tiny structures based on these same physical principles.

However, a PN Junction alone is not enough to build a processor.

A CPU needs a switch that can be controlled by another electrical signal.

That switch is the **MOSFET**.

In the next section, we'll finally study the transistor itself—its **Gate**, **Source**, and **Drain**—and see exactly how applying voltage to the Gate creates or destroys a conductive channel.