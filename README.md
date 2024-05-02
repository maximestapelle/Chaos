# Chaos
## Bifurcation graphs, Lyapunov exponents and Attractors for Dynamical systems in C/Python
This is Chaos, a command-line program that will generate plots of Attractors, Bifurcation diagrams, and Lyapunov exponents for a variety of dynamical systems (flows and maps).

Nothing really fancy or especially new here,  I'm doing this as an exercise in coding and physics.

The application is written in C, and it uses
-  Python 3 - matplotlib (at least 3.7) and LaTeX for the plots;
-  A SQLITE3 database to store parameters about the dynamical systems;

It should compile on MacOSX & Linux, provided that the above are installed. It will then create an executable named Chaos.

### Installation guide of dependencies for Linux users :
Install Python3, matplotlib + ability to export to png
```console
sudo apt-get install python3
sudo apt-get install python3-pip
python3 -m pip install -U matplotlib
sudo apt-get install dvipng
```
Install LaTeX
```console
sudo apt-get install texlive-base
sudo apt-get install texlive-recommended
sudo apt-get install texlive-extra
sudo apt-get install cm-super
```
Install SQLITE3 + development tools
```console
sudo apt-get install sqlite3
sudo apt-get install libsqlite3-dev
```
