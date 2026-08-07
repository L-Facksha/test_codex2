*This project has been created as part of the 42 curriculum by azebahad.*

# Codexion

## Description

Codexion is a multithreaded simulation of coders racing against the clock to
share a limited pool of hardware. A number of **coders** sit around a
circular co-working hub with a shared Quantum Compiler at its center. Between
each pair of neighboring coders sits one **USB dongle**  there are always as
many dongles as coders. To compile, a coder needs **both** their left and
right dongle held simultaneously; once compiling finishes, both dongles go
back on the table and the coder moves on to debugging, then refactoring,
before immediately trying to acquire dongles again.

The goal of the project is to implement this simulation entirely with POSIX
threads (`pthread`), building a correct, fair, and deadlock-free arbitration
system from scratch:

- One thread per coder, plus a dedicated monitor thread.
- A custom binary-heap priority queue (no standard library priority queue) per
  dongle, arbitrating access with either **FIFO** or **EDF (Earliest Deadline
  First)** scheduling.
- A mandatory **cooldown** period before a released dongle can be taken again.
- Precise **burnout** detection: if a coder does not start compiling within
  `time_to_burnout` milliseconds of the start of their last compile (or of the
  simulation), the whole simulation stops.
- The simulation ends either when every coder has completed
  `number_of_compiles_required` compiles, or the instant any coder burns out.

Coders never communicate with each other and have no way of knowing whether a
neighbor is close to burning out all coordination happens purely through
the shared dongles and their scheduling state.

## Instructions

### Compilation

```sh
make        # builds the "codexion" binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

Compiled with `cc -Wall -Wextra -Werror -pthread`.

### Running

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument                      | Meaning                                                                 |
|--------------------------------|--------------------------------------------------------------------------|
| `number_of_coders`             | Number of coders **and** number of dongles                              |
| `time_to_burnout` (ms)         | Max time without starting a compile before a coder burns out            |
| `time_to_compile` (ms)         | Duration of the compiling phase (requires both dongles)                 |
| `time_to_debug` (ms)           | Duration of the debugging phase                                         |
| `time_to_refactor` (ms)        | Duration of the refactoring phase                                       |
| `number_of_compiles_required`  | Compiles needed per coder for the simulation to end successfully        |
| `dongle_cooldown` (ms)         | Time a dongle stays unavailable after being released                    |
| `scheduler`                    | `fifo` or `edf` arbitration policy used by each dongle                |

Example:

```sh
./codexion 4 3000 200 150 150 5 100 edf
```

All arguments are mandatory and validated; invalid input (negative numbers,
non-integers, or an unknown scheduler) is rejected with an error message.

### Output format

Every state change is logged as `timestamp_in_ms coder_id event`:

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
```

Taking the two dongles needed to compile always produces two
`"has taken a dongle"` lines before the matching `"is compiling"` line.
Output is serialized so lines from different coders never interleave.

## Resources

- *man pages*: `pthread_create(3)`, `pthread_mutex_lock(3)`,
  `pthread_cond_wait(3)`, `pthread_cond_timedwait(3)`, `pthread_cond_signal(3)`,
  `gettimeofday(2)`, `clock_gettime(2)`
- <https://youtu.be/d9s_d28yJq0?si=r2H_P6SR03toTYqa>
- <https://man7.org/linux/man-pages/man7/pthreads.7.html>
- <https://www.mathworks.com/products/polyspace/static-analysis-notes/what-data-races-how-avoid-during-software-development.html>
- <https://runestone.academy/ns/books/published/pythonds/Trees/BinaryHeapImplementation.html>

### AI usage

An AI assistant was used during this project to help
understand POSIX threading concepts explaining primitives such as
`pthread_t`, `pthread_mutex_t`, and `pthread_cond_t`, how they interact, and
how thread processes and synchronization work in general, before writing and
implementing the code myself.

## Blocking cases handled

**Deadlock prevention (Coffman's conditions).** A deadlock needs all four
conditions at once; this design breaks two of them:
- *Mutual exclusion* kept (a dongle can only be held by one coder), enforced
  by a per-dongle mutex.
- *Hold and wait* **broken**: both dongles are locked together and granted
  atomically, either both at once or neither.
- *No preemption* not needed once hold-and-wait is broken.
- *Circular wait* **broken**: every coder always locks the lower-id dongle
  first, so the lock order is the same for everyone.

**Starvation prevention.** Each dongle keeps pending requests in a priority
heap. Under `edf`, a coder close to burning out is moved ahead of coders who
asked earlier but have more time left, so no coder is starved forever.

**Cooldown handling.** A released dongle stores `last_released_at` and stays
ungrantable until `dongle_cooldown` ms pass checked in the same atomic step
as the heap-order check, so it can't be bypassed by a race.

**Precise burnout detection.** A monitor thread polls every millisecond and
compares `now - last_compile_start` to `time_to_burnout` for each waiting
coder, keeping the burnout log within the required 10 ms window.

**Log serialization.** Every log line is printed inside one mutex, so two
coders' lines can never interleave.

## Thread synchronization mechanisms

| Primitive | Where | Purpose |
|---|---|---|
| `pthread_mutex_t` (per dongle) | dongle struct | Protects `taken`, `last_released_at`, and the pending-request heap |
| `pthread_cond_t` (per dongle) | dongle struct | Lets a waiting coder sleep instead of busy-polling |
| `pthread_mutex_t` (state) | config struct | Protects shared coder state read by the monitor |
| `pthread_mutex_t` (print) | config struct | Serializes log output |

**Preventing race conditions on a dongle.** `taken` is only ever read or
written while its mutex is held, so the check and the take happen in one
atomic step two coders can never both see it free and both grab it:

```c
pthread_mutex_lock(&left->mutex);
pthread_mutex_lock(&right->mutex);
if (can_grant_pair(coder, left, right))
{
    left->taken = 1;
    right->taken = 1;
}
pthread_mutex_unlock(&right->mutex);
pthread_mutex_unlock(&left->mutex);
```

**Thread-safe communication between coders and the monitor.** Coders and the
monitor thread share each coder's phase, `last_compile_start`, and
burned-out flag through the same state mutex the monitor always reads a
consistent, up-to-date value, never a stale or half-written one. When a
dongle is released, `pthread_cond_broadcast()` wakes any coder waiting on it:

```c
pthread_mutex_lock(&dongle->mutex);
dongle->taken = 0;
dongle->last_released_at = get_time_ms();
pthread_cond_broadcast(&dongle->scheduler.cond);
pthread_mutex_unlock(&dongle->mutex);
```