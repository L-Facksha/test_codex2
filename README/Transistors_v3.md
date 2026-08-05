---

# Doping

In the previous section we discovered an important limitation.

Pure silicon is beautiful.

Pure silicon is stable.

But pure silicon is **not useful enough** for building modern processors.

The reason is simple.

Almost every electron is locked inside a covalent bond.

```
Intrinsic Silicon

Electron

↓

Locked

↓

Cannot move
```

Very few free electrons exist.

Current barely flows.

Engineers needed a way to create **more charge carriers** without destroying the crystal structure.

The solution was one of the greatest discoveries in semiconductor engineering.

It is called

**Doping.**

---

# What is Doping?

Doping is the process of intentionally replacing a very small number of silicon atoms with atoms from another element.

Imagine one million silicon atoms.

```
Si Si Si Si Si Si ...

...

1,000,000 atoms
```

Now replace just one or two of them.

```
Si Si Si Si P Si ...

...

1,000,000 atoms
```

Notice something.

Almost the entire crystal is still silicon.

Only a tiny fraction has changed.

Yet this tiny modification completely changes how electricity moves.

---

# Why Does One Atom Matter?

You might think,

"How can changing one atom affect millions of others?"

Think about a classroom.

Suppose every student has exactly one pencil.

Everyone is perfectly balanced.

```
🙂 ✏️

🙂 ✏️

🙂 ✏️

🙂 ✏️
```

Now one student suddenly has two pencils.

```
🙂 ✏️✏️

🙂 ✏️

🙂 ✏️

🙂 ✏️
```

There is now one extra pencil.

That extra pencil can easily be given to someone else.

The same thing happens inside silicon.

A single extra electron becomes free to move through the crystal.

---

# Two Types of Doping

There are only two major ways to dope silicon.

```
Doping

├── N-Type

└── P-Type
```

Each changes electricity differently.

---

# N-Type Silicon

Let's start with N-Type.

Instead of replacing a silicon atom with another silicon atom,

we replace it with

**Phosphorus (P).**

Why phosphorus?

Because phosphorus has

```
5

valence electrons.
```

Silicon has

```
4

valence electrons.
```

Let's compare.

Silicon

```
      ●

   ●  Si  ●

      ●
```

Phosphorus

```
      ●

   ●  P  ●

    ●
```

Notice the difference.

Phosphorus has

**one extra electron.**

---

# Replacing One Silicon Atom

Originally,

our crystal looked like this.

```
Si —— Si —— Si

|      |      |

Si —— Si —— Si

|      |      |

Si —— Si —— Si
```

Now replace one silicon atom.

```
Si —— Si —— Si

|      |      |

Si —— P —— Si

|      |      |

Si —— Si —— Si
```

The phosphorus atom still forms four covalent bonds.

```
Bond

Bond

Bond

Bond
```

But...

It still has

```
One Electron Left
```

---

# The Extra Electron

That fifth electron has nowhere to bond.

It is only weakly attached to the phosphorus atom.

```
P

↓

5 electrons

↓

4 used

↓

1 left over
```

That leftover electron is almost free.

A tiny amount of energy is enough to release it.

```
Before

P

↓

Extra Electron

↓

Bound

-------------------

After

P

↓

Electron →

Free
```

Now electricity can move much more easily.

---

# Donor Atom

Phosphorus is called a

**Donor.**

Why?

Because it donates an electron.

```
Phosphorus

↓

Gives Electron

↓

Current increases
```

This is why N-Type silicon contains many free electrons.

---

# Majority Carriers

Every semiconductor has two types of charge carriers.

- Electrons
- Holes

In N-Type silicon,

electrons are much more numerous.

Therefore,

electrons are called

**Majority Carriers.**

Holes still exist,

but there are far fewer of them.

They are called

**Minority Carriers.**

---

# Why Is It Called N-Type?

The letter

```
N
```

stands for

```
Negative.
```

Not because the silicon itself is negatively charged.

The material remains electrically neutral.

It is called N-Type because

its current is carried mainly by

**negative electrons.**

---

# P-Type Silicon

Now let's look at the opposite situation.

Instead of phosphorus,

engineers use

**Boron (B).**

Why boron?

Because boron has only

```
3

valence electrons.
```

Silicon has

```
4
```

Let's compare.

Silicon

```
      ●

   ●  Si  ●

      ●
```

Boron

```
      ●

   ●  B  ●
```

Notice something.

One electron is missing.

---

# Replacing a Silicon Atom

Again,

replace one silicon atom.

```
Si —— Si —— Si

|      |      |

Si —— B —— Si

|      |      |

Si —— Si —— Si
```

Boron wants four bonds,

but it only has three electrons.

```
Bond

Bond

Bond

❌ Missing
```

One bond cannot be completed.

A hole appears.

---

# What Is a Hole?

A hole is simply

```
A missing electron.
```

Imagine a parking lot.

```
🚗 🚗 🚗 🚗

🚗 🚗 ⬜ 🚗

🚗 🚗 🚗 🚗
```

The empty parking space behaves almost like an object.

Cars move into it.

The empty space appears to move.

Exactly the same thing happens with holes.

Electrons continuously jump into the hole.

```
Before

● ○

↓

After

○ ●
```

Although electrons are moving,

we often describe the hole as moving.

This simplifies circuit analysis.

---

# Acceptor Atom

Boron is called an

**Acceptor.**

Why?

Because it accepts an electron from neighboring atoms.

```
Neighbor Electron

↓

Moves

↓

Hole Filled
```

But now

the neighboring atom has a hole.

The hole seems to travel through the crystal.

---

# Majority Carriers

In P-Type silicon,

holes are much more numerous than free electrons.

Therefore,

holes become

**Majority Carriers.**

Electrons become

**Minority Carriers.**

---

# Why Is It Called P-Type?

The letter

```
P
```

stands for

```
Positive.
```

Again,

the material itself is not positively charged.

It remains electrically neutral.

It is called P-Type because

its current is mainly described as the movement of

**positive holes.**

---

# Comparing N-Type and P-Type

| Property | N-Type | P-Type |
|----------|---------|---------|
| Dopant | Phosphorus | Boron |
| Valence Electrons | 5 | 3 |
| Majority Carrier | Electrons | Holes |
| Minority Carrier | Holes | Electrons |
| Also Called | Donor | Acceptor |

---

We now have two completely different semiconductor materials.

One is rich in electrons.

The other is rich in holes.

The next question is obvious.

> **What happens if we connect them together?**

That single question leads to the invention of the **PN Junction**, the heart of every diode and every transistor.