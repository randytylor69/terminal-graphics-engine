## Termigine (*Terminal Graphics Engine*)

To render a mysterious rotating cube, or a super bouncy orb, this graphics engine does it all by spitting ASCII characters all over the terminal. Illusions of 3D graphics is achieved by rigorous usage of linear algebra and basic trigonometry; lengthy arithmetic operations regarding such are functionalized in `Termigine.h`. 

&nbsp;

## Build

1. Only 2 files are required:

- `termigine.h`
- `termigine.cpp`

Everything else in this folder exists to demonstrate the engine's capabilities.

2. Include `termigine.h` and instantiate an `Engine` class object to access all its methods:

```cpp
#include "termigine.h"

Engine engine;

/* example method: wiping everything off the terminal */
engine.clearScreen();
```

3. I'm using `Makefile`, so the `termigine` binary is built with:

```make
termigine.o : termigine.cpp
    g++ -std=c++23 -c $^ -o termigine.o
```

Where the `std=c++23` flag is required to access `std::print`.

&nbsp;

## Philosophy

- Maximized usage of the C++ standard library (i.e. `std::print`, `std::vector`, etc.)
- Minimized external dependencies
- Unecessarily long and detailed documentation on Obsidian
- Write everything from scratch (as much as possible)
- Write code that is easy to read, debug (i.e. `printf(" --> im here\n");`), and update
- ... 

&nbsp;

## ScreenShots

| ![Triangle](./screenshots/newscreenshot_045.jpg) | ![Cube](./screenshots/newscreenshot_052.jpg)
|---|---|
| A tilted triangle | A rotating cube (the image, of course, failed to capture its movement) |


