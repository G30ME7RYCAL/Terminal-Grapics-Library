
# Terminal Graphics Library

A header-only C++ 2D graphics engine that renders to the terminal, has image export with pixmap and bitmap, and an HLSL-style shading language.

![Logo](images/logo.png)

## Prerequisites
| Requirement | Details |
|---|---|
| **Compiler** | C++17 compiler (GCC 8+, Clang 7+, or MSVC 2019+) |
| **Dependencies** | None, std only |
| **Terminal** | Must support 24-bit truecolor ANSI escape codes (kitty, iterm2, Windows Terminal, GNOME Terminal, Kitty, Alacritty, WezTerm, VS Code's terminal, and most modern Linux terminals. |
| **Display** | at least width of 2 * window width columns (pixels are 2 characters wide) |
| **Otimizations** | 1 : 2 character w : h aspect ratio for square pixels, terminal support for synchronized updates (`ESC[?2026h`) removes flicker (ignored if unsupported) |

## Installation
TGL is header-only. Copy the folder inside your project directory or clone.
```sh
    git clone https://github.com/G30ME7RYCAL/Terminal-Grapics-Library.git
```

```
workspace/
├── main.cpp
└── TGL/
    ├── tgl.h              <- include this one
    ├── tgl_constants.h
    ├── tgl_draw.h
    ├── tgl_geometry.h
    ├── tgl_log.h
    ├── tgl_shader.h
    ├── tgl_styles.h
    └── TSL/
        ├── tsl.h
        ├── tgl_vector.h
        └── tgl_matrix.h
```

the `TSL/` directory must sit *inside* of `TGL/` because `tgl.h` and `tgl_styles.h` include `"TSL/tsl.h"` by relative path.


## Getting started
```sample.cpp
#include "TGL/tgl.h"

int main() {
    if (!tglInit()) {
        std::cerr << "ERROR Initializing TGL: " << pullErr() << std::endl;
    }

    TGLwindow SCREEN {WIDTH, HEIGHT, TGL_BLUE}; // Window surface with a blue background

    if (!tglCreateWindow(SCREEN)) {
        std::cerr << "ERROR Creating Window Array: " << pullErr() << std::endl;
    }

    // Orange triangle
    TGLpolygon tri;
    tri.vertices = { {5, 17}, {30, 2}, {55, 17} };
    tri.TGL_Fill_Color   = TGL_ORANGE;
    tri.TGL_Border_Color = TGL_WHITE;
    tglRasterizePolygon(tri);

    // Shade with a horizontal red to blue gradient
    tglFragmentShader(tri, [](TGLpoint &p, TGLpolygon &) {
        return lerp(TGL_RED, TGL_BLUE, p.x / 60.0f);
    });

    // Draw triangle and display
    tglDrawPolygon(tri, window);
    tglUpdateDisplay(window); //write to the terminal
    //tglUpdateDisplay(window, "image.ppm"); //or to a display image
}
```

Build and run:

```sh
g++ -std=c++17 -O2 sample.cpp -o sample
./sample
```
