This is an example game for an in-progress 2D game engine.

Getting Started
---------------
Run the following commands to get your project ready to use with msys2 on Windows. **Not tested on a clean PC.**
**Make sure to clone using ssh**
```
pacman -S git cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-boost mingw-w64-x86_64-sfml mingw-w64-x86_64-box2d mingw-w64-x86_64-jsoncpp
git clone git@github.com:xxAtrain223/ArkAngel.git
cd ArkAngel
git submodule update --init
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
