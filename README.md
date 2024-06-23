# Chaos
## Bifurcation graphs, Lyapunov exponents and Attractors for Dynamical systems in C/Python
This is Chaos, a command-line program that will generate plots of Attractors, Bifurcation diagrams, and Lyapunov exponents for a variety of dynamical systems (flows and maps).

I'm doing this as an exercise in coding and physics.

The application is mainly written in C, and it uses
-  Python 3 - matplotlib (at least 3.7) and LaTeX for the plots;
-  A SQLITE3 database to store parameters about the dynamical systems.

I've provided a makefile for an easy installation on Linux and MacOS (see [`INSTALL.md`](./INSTALL.md) for instructions). I haven't tried to build Chaos on Windows.

### Bifurcation

<figure>
    <img src="/doc/img/Bifurcation example.png"
         alt="An example of Bifurcation graph (The Rössler system)">
    <figcaption>An example of Bifurcation graph (The Rössler system)</figcaption>
</figure>

### Lyapunov exponents

<figure>
    <img src="/doc/img/Lyapunov example.png"
         alt="An example of Lyapunov exponents (The Lorenz system)">
    <figcaption>An example of Lyapunov exponents (The Lorenz system)</figcaption>
</figure>

### Attractors

<figure>
    <img src="/doc/img/Attractor example.png"
         alt="An example of Attractor (The Tinkerbell map)">
    <figcaption>An example of Attractor (The Tinkerbell map)</figcaption>
</figure>
