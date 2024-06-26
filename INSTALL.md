# Installation guide

There are dependencies (on top of a recent C compiler, of course):
- Python and matplotlib (at least 3.7);
- A working LaTeX Installation + full Computer Modern Fonts + dvi to png;
- The RDBMS SQLITE3 with development tools.
- `uthash` for lookup tables and hash functions.

If those are already present, Chaos should work fine. A Makefile is provided with `gcc` as the default compiler (Chaos compiles successfully with `clang` as well). After downloading the code and entering the local directory, simply run make:
```
cd Chaos
make
```

This will create an executable `Chaos`, just run it! :smile:
```
./Chaos
```

---

If needed, a way to install the dependencies is described below.

## Installing dependencies


### Linux

> with the package manager `apt-get`:

*Install Python3, matplotlib*
```
sudo apt-get install python3
sudo apt-get install python3-matplotlib
```
*Install* LaTeX *with* TexLive
```
sudo apt-get install texlive-latex-base
sudo apt-get install texlive-latex-recommended
sudo apt-get install texlive-latex-extra
sudo apt-get install cm-super
sudo apt-get install dvipng
```
*Install* SQLITE3 *+ development tools*

```
sudo apt-get install sqlite3
sudo apt-get install libsqlite3-dev
```
*Install* uthash

```
sudo apt-get install uthash-dev
```

### MacOS X

> My preferred package manager is [MacPorts](https://www.macports.org/install.php), so I detail the procedure with MacPorts installed.

**Note**: If you have error messages related to Xcode command line developer tools, the best is to install xcode with `xcode-select --install`.

*Install Python3, matplotlib*
> Replace 312 below by the [wanted release of Python](https://www.python.org/downloads/) (>=3.7). You can also type `port list | grep matplotlib` to list all available python version with matplotlib, and install the most recent one.

```
sudo port install py312-matplotlib
sudo port select --set python3 python312
```

*Install* LaTeX *with* TexLive
```
sudo port install texlive texlive-latex-extra
```

*Install* SQLITE3 *+ development tools*

> sqlite3 is already installed thanks to the python installation.

*Install* uthash

```
sudo port install uthash
```
Since MacPorts will install this in `/opt/local/include/uthash`, it is necessary to tell the C compiler where to read the header file. I find it the most convenient to symlink the file into Chaos' include directory (type this command when you're in the main directory of Chaos):
```
ln -s /opt/local/include/uthash/uthash.h src/include/
```
