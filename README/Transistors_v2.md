---

# Crystal Structure

Now that we know **why silicon is used**, we need to understand something even more important:

> **Why does electricity behave differently inside silicon than inside copper?**

The answer is hidden in the way atoms arrange themselves.

A silicon atom has **4 valence electrons**.

Valence electrons are the electrons in the outermost shell of an atom.

They are the electrons responsible for forming bonds with neighboring atoms.

Imagine a single silicon atom.

```
           ●

      ●    Si    ●

           ●
```

The four surrounding dots represent the four valence electrons.

One silicon atom alone is unstable.

Nature always tries to make atoms stable.

To become stable, silicon shares its electrons with neighboring silicon atoms.

---

# Covalent Bonds

Imagine two silicon atoms.

```
Si        Si
```

Each has four valence electrons.

Instead of keeping all their electrons,

they share them.

```
      ●

Si —— ● —— Si

      ●
```

This shared pair of electrons is called a

**Covalent Bond.**

Every silicon atom forms four covalent bonds.

Eventually millions of atoms connect together.

```
Si —— Si —— Si

|      |      |

Si —— Si —— Si

|      |      |

Si —— Si —— Si
```

This repeating pattern is called a

**Crystal Lattice.**

---

# Why Electrons Cannot Move

Look carefully.

Every electron already belongs to a bond.

```
Electron

↓

Holding two atoms together.
```

If an electron tried to leave,

the bond would break.

Nature doesn't like unstable structures.

Therefore,

inside pure silicon,

almost no electrons are free.

```
Electron

↓

Locked

↓

Cannot move freely
```

That is why pure silicon is **not a good conductor.**

---

# Copper vs Silicon

Copper behaves very differently.

Copper has one loosely bound outer electron.

```
Cu

↓

Free Electron
```

Those electrons move almost immediately when voltage is applied.

```
Copper

Electron →

Electron →

Electron →
```

Silicon,

however,

keeps almost every electron trapped inside covalent bonds.

```
Silicon

Electron

↓

Locked
```

This is why silicon conducts electricity much more poorly than copper.

---

# Pure Silicon

Engineers call undoped silicon

**Intrinsic Silicon.**

Intrinsic silicon contains

- No impurities
- No extra electrons
- No missing electrons

Every atom is perfectly balanced.

```
Si —— Si —— Si

|      |      |

Si —— Si —— Si

|      |      |

Si —— Si —— Si
```

This perfect structure is beautiful,

but not useful for building processors.

Why?

Because almost no current can flow.

---

# Temperature Changes Everything

Imagine heating the silicon.

As temperature increases,

atoms vibrate more.

```
Cold

↓

Atoms almost still

---------------

Hot

↓

Atoms vibrate
```

Sometimes,

one electron gains enough energy to escape its bond.

```
Before

Si —— Si

After

Si     Si

   Electron →
```

Now we have something new.

A free electron.

And an empty space where the electron used to be.

That empty space is called a

**Hole.**

---

# Electron and Hole

Whenever an electron escapes,

two things are created.

```
Electron

↓

Free

-------------------

Hole

↓

Missing Electron
```

The hole behaves as if it were a positive particle.

Although nothing physically moved into the hole,

another nearby electron can jump into it.

```
Before

A ● B ○

↓

After

A ○ B ●
```

From our point of view,

it looks like the hole moved.

This idea is extremely important.

In semiconductor physics,

we often describe current as the movement of

- Electrons
- Holes

Both contribute to electrical conduction.

---

# Energy Bands

To understand why electrons escape,

we need one more idea.

Electrons are only allowed to exist at specific energy levels.

Think of a building.

```
Floor 2

-----------------

Floor 1
```

An electron cannot float between floors.

It must stay on one floor.

Inside solids,

these floors become

**Energy Bands.**

The two important bands are

```
Conduction Band

----------------

Band Gap

----------------

Valence Band
```

---

# Valence Band

The Valence Band contains the electrons forming covalent bonds.

```
Valence Band

██████████████
```

Those electrons cannot move freely.

---

# Conduction Band

The Conduction Band contains free electrons.

```
Conduction Band

● ● ● ● ●
```

These electrons can travel through the material.

Current becomes possible.

---

# Band Gap

Between these two bands is an empty region.

```
Conduction Band

──────────────

Band Gap

──────────────

Valence Band
```

The Band Gap is an energy barrier.

Electrons need enough energy to jump across it.

---

# Conductors

In metals,

the bands overlap.

```
Conduction Band

██████████

Valence Band

██████████
```

Electrons move easily.

Current flows immediately.

---

# Insulators

In insulators,

the Band Gap is enormous.

```
Conduction Band

──────────────

Huge Gap

──────────────

Valence Band
```

Almost no electron can cross.

Current cannot flow.

---

# Semiconductors

Silicon sits between these extremes.

Its Band Gap is small.

```
Conduction Band

──────────────

Small Gap

──────────────

Valence Band
```

Normally,

very few electrons cross.

But if we add

- Heat
- Light
- Voltage

or modify the material,

many more electrons can reach the conduction band.

This makes silicon controllable.

And that controllability is exactly what engineers wanted.

---

# Why Pure Silicon Is Still Not Enough

Although silicon is controllable,

it still doesn't conduct well enough.

A modern processor needs billions of transistors switching billions of times every second.

Intrinsic silicon cannot do this efficiently.

We need a way to increase the number of charge carriers.

We do this by intentionally modifying the crystal.

This process is called

**Doping.**

In the next section, we'll discover how replacing **one atom out of millions** completely changes the electrical behavior of silicon and makes modern computers possible.