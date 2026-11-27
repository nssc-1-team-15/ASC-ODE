# Welcome to Team 15 ASC-ODE's documentation!

ASC-ODE is a small C++ library for numerically solving initial-value problems for ordinary differential equations (ODEs). It represents an ODE in the form

\[
  y'(t) = f(y(t)), \qquad y(t_0) = y_0,
\]

and advances the state vector \(y\) in discrete time steps. The right-hand side is supplied as a `NonlinearFunction`; time-stepping methods implement the `TimeStepper` interface.

The repository includes explicit and implicit Euler methods, an improved Euler method, and a general implicit Runge–Kutta implementation. The Runge–Kutta implementation can use coefficient tables such as Gauss–Legendre and Radau IIA. See [Time-stepping methods](timesteppers.md) for an overview and usage notes.

## Mass–spring example

The demo `demos/test_ode.cpp` solves a unit-mass, unit-stiffness oscillator. With position \(q\) and velocity \(v\), its first-order system is

\[
  \begin{aligned}
  q' &= v, \\
  v' &= -q.
  \end{aligned}
\]

The state is stored as a two-element vector, `y = {q, v}`. The demo initializes it to `{1, 0}`, advances it repeatedly with `stepper.doStep(tau, y)`, and writes time, position, and velocity to `output_test_ode.txt`. The phase plot below shows velocity against position for a mass–spring simulation.

```{image} ../pictures/massspring_phase.png
:alt: Phase plot of the mass-spring oscillator, with velocity plotted against position
:width: 65%
:align: center
```

## Build and run the demo

From the repository root, initialize the `nanoblas` submodule and build the C++ examples:

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
./build/test_ode
```

The executable writes its numerical results to `output_test_ode.txt`. The plotting script is `../../plot/plotmassspring.py`; it requires NumPy and Matplotlib, uses the shared style in `../../plot`, and saves generated figures under `../../plot`. The documentation reference images remain in `docs/pictures/`. For the live documentation preview, run `jupyter book start` from `docs/`.

## Project links

- [Source repository](https://github.com/nssc-1-team-15/ASC-ODE)
- [Time-stepping methods](timesteppers.md)
- [Background on ODEs](https://jschoeberl.github.io/IntroSC/ODEs/ODEs.html)
