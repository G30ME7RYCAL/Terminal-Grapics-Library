# Window & Display
### **_Section: 01_**
<br><br>
## `tglInit();`
Every TGL program has to start with `tglInit()` to flush the terminal, reset styles, and prepare for output.
<br><br>
## The `TGLwindow` class
To draw anything in TGL, you must create a `TGLwindow` object with `TGL_Window_Width`, `TGL_Window_Height`, and `TGL_Window_Style`, (more on that in chapter 3). Once you created the object, initialize create the pixels and initialize values using `tglCreateWindow()` and pass it your `TGLwindow` object to initialize the window object for output.
<br><br>
## Displaying your Window
To display your window, call `tglUpdateDisplay()` and pass it your window. To output into an image file, pass it a file path which is either pixmap (.ppm) or bitmap (.bmp). Calling it multiple times will overwrite the previous output, creating a continuous animation.
<br><br>
## Sample code
Try it out! Create and display a blue window to the terminal.
```cpp
#include "TGL/tgl.h"

int main() {
    if (!tglInit()) {
        std::cerr << "ERROR Initializing TGL: " << pullErr() << std::endl;
    }

    TGLwindow SCR {100, 100, TGL_BLUE}; // 100px * 100px window surface with a blue background

    if (!tglCreateWindow(SCR)) {
        std::cerr << "ERROR Creating Window Array: " << pullErr() << std::endl;
    }

    tglUpdateDisplay(SCR); //write to the terminal
    //tglUpdateDisplay(SCR, "image.ppm"); //or to a display image
}
```
