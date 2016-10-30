This is an example game for an in-progress 2D game engine.

Getting Started
---------------

### Windows - MSYS2

Here is an example of downloading, building, and running ArkAngel in Windows using MSYS2.

Prerequisites:

- [MSYS2](http://msys2.github.io/) has been installed and updated.
- Git has been installed and configured (`pacman -S git`).
- An SSH key has been [added to your Github account](https://help.github.com/articles/generating-an-ssh-key/).

Download:

```
$ pacman -S cmake mingw-w64-x86_64-{toolchain,ninja,boost,sfml,box2d,jsoncpp}
$ cp /mingw64/share/SFML/cmake/Modules/FindSFML.cmake /mingw64/share/cmake-3.6/Modules/ # Workaround for package error
$ git clone git@github.com:xxAtrain223/ArkAngel.git
$ cd ArkAngel
$ git submodule update --init --recursive
```

Build:

```
$ mkdir build
$ cd build
$ cmake -G 'Ninja' ..
$ ninja ArkAngel
$ cd ..
```

Run:

```
$ build/ArkAngel.exe
```

Technologies
------------

* **CMake-** An open-source, cross-platform family of tools designed to build, test and package software.
* **Ginseng-** An entity component framework designed for use in games.
* **Raspberry-** A lightweight C++ type erasure library.
* **Bait-** A behavior tree library.
* **Box2D-** A feature rich 2D rigid body physics engine.
* **AngelScript-** An extremely flexible cross-platform scripting library designed to allow applications to extend their functionality through external scripts.
* **SFML-** Simple and Fast Multimedia Library; SFML is a simple, fast, cross-platform and object-oriented multimedia API. It provides access to windowing, graphics, audio and network.
* **SFGUI-** Simple and Fast Graphical User Interface; SFGUI is a fast, simple and native C++ GUI library for SFML. It provides a rich set of widgets and is highly customizable in its looks.
* **JsonCpp-** a C++ library that allows manipulating JSON values, including serialization and deserialization to and from strings.
