*This project has been created as part of the 42 curriculum by amostash.*

# Codexion

A multithreaded concurrency simulation written in C that models multiple coders competing for limited shared resources (USB dongles). The project focuses on synchronization, thread safety, deadlock prevention, fairness, and precise timing using POSIX threads.

---

## Description

Codexion is inspired by the classic Dining Philosophers problem but introduces additional synchronization challenges.

Each coder alternates between:

- Compiling
- Debugging
- Refactoring

To compile, a coder must acquire **two USB dongles**. Since dongles are shared between neighboring coders, proper synchronization is required to avoid:

- Deadlocks
- Race conditions
- Starvation
- Resource contention

The simulation ends when:

- A coder burns out (fails to compile before the timeout), or
- Every coder has compiled the required number of times.

The project is implemented using **POSIX threads (pthread)** and synchronization primitives while respecting the 42 coding standard.

---

# Features

- POSIX thread implementation
- Mutex-protected shared resources
- Fair resource scheduling
- FIFO scheduler
- EDF (Earliest Deadline First) scheduler
- Monitor thread for burnout detection
- Thread-safe logging
- USB dongle cooldown support
- Deadlock prevention
- Starvation prevention (EDF mode)
- Clean memory management
- No global variables

---

# Project Structure

```
.
├── README.md
└──coders/
	├── include
	|	└── *.h
	|
	├── src
	|	└── *.c
	|
	└── Makefile
```

---

# Compilation

Compile the project using:

```bash
make
```

Clean object files:

```bash
make clean
```

Remove executable and objects:

```bash
make fclean
```

Rebuild:

```bash
make re
```

---

# Usage

```
./codexion \
number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

Example:

```bash
./codexion 5 800 200 200 200 5 50 fifo
```

or

```bash
./codexion 5 800 200 200 200 5 50 edf
```

---

# Program Arguments

| Argument | Description |
|-----------|-------------|
| number_of_coders | Number of coder threads |
| time_to_burnout | Maximum time before a coder burns out |
| time_to_compile | Time spent compiling |
| time_to_debug | Time spent debugging |
| time_to_refactor | Time spent refactoring |
| number_of_compiles_required | Simulation ends after everyone compiles this many times |
| dongle_cooldown | Time before a released dongle becomes available again |
| scheduler | `fifo` or `edf` |

---

# Example Output

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
402 2 has taken a dongle
403 2 has taken a dongle
403 2 is compiling
...
1204 3 burned out
```

---

# Blocking Cases Handled

This implementation addresses the major synchronization problems found in concurrent systems.

## Deadlock Prevention

- Circular wait is prevented through controlled resource acquisition.
- Dongles are never duplicated.
- Shared resources are always released correctly.
- Resource ownership is protected with mutexes.

## Race Conditions

All shared data is protected using mutexes, including:

- Dongles
- Simulation state
- Logging
- Burnout detection
- Compile counters

## Starvation Prevention

When using **EDF scheduling**, waiting coders are prioritized according to the earliest burnout deadline, ensuring fair access to resources whenever possible.

## Dongle Cooldown

Each USB dongle enters a cooldown period after being released. During this interval it cannot be acquired by another coder.

## Precise Burnout Detection

A dedicated monitor thread continuously checks coder deadlines and reports burnout within the required timing tolerance.

## Serialized Logging

Console output is protected by a logging mutex so messages never interleave between threads.

---

# Thread Synchronization Mechanisms

The project uses several POSIX synchronization primitives.

## pthread_mutex_t

Used to protect shared resources including:

- USB dongles
- Shared simulation state
- Console output
- Compile counters

## pthread_cond_t

Condition variables are used to coordinate waiting coders and wake them when resources become available.

## Monitor Thread

A dedicated monitoring thread:

- checks burnout deadlines,
- stops the simulation,
- ensures only one termination event occurs.

## Scheduling Policies

### FIFO

Requests are served in arrival order.

Suitable for simple fairness.

### EDF (Earliest Deadline First)

Requests are ordered by:

```
last_compile_start + time_to_burnout
```

The coder closest to burnout receives priority.

---

# Technical Concepts

This project demonstrates practical use of:

- POSIX Threads
- Mutexes
- Condition Variables
- Thread-safe programming
- Resource arbitration
- Synchronization
- Scheduling algorithms
- Priority queues
- Timing
- Concurrency
- Deadlock avoidance
- Starvation prevention

---

# Resources

## POSIX Threads

- https://man7.org/linux/man-pages/man7/pthreads.7.html

## Mutexes

- https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html

## Condition Variables

- https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html

## Dining Philosophers

- https://en.wikipedia.org/wiki/Dining_philosophers_problem

## Scheduling Algorithms

- Earliest Deadline First (EDF)
- FIFO Scheduling

---

# AI Usage

AI was used as a learning assistant during development for:

- understanding POSIX thread synchronization,
- reviewing concurrency concepts,
- improving documentation,
- explaining scheduling algorithms,
- editing README documentation.

All implementation, debugging, testing, and final verification were completed and understood by the project author.

---

# Learning Outcomes

Through this project I gained experience with:

- Concurrent programming
- Thread synchronization
- Mutexes and condition variables
- Deadlock avoidance
- Fair scheduling
- Timing-sensitive applications
- Thread-safe logging
- Resource management
- Designing multithreaded software

---
