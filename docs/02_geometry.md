# Geometry
### **_Section: 02_**
<br><br>
## Your first point
To draw a point to the screen, you must create a `TGLpoint` object, which has an X coordinate, Y coordinate, and style.
You can initialize one like this:
```cpp
TGLpoint redPoint { 10, 35, TGL_RED };
```
To draw your point, call
```cpp
tglDrawPoint(redPoint, SCR);
```
SCR is the TGLwindow object we created [Chapter 01](docs/01_window-and-display.md).

## A line
You can create a line segment by creating a `TGLseg` object, and giving a start point and an end point.
For example:
```cpp
TGLseg greenLine { redPoint, TGLpoint { 40, 50 }, TGL_GREEN };
```
Before you can draw a line to the window, you need to rasterize it so the computer knows which pixels to color.
In TGL, you do this by calling `tglRasterizeSeg()` and passing your `TGLseg` object
```cpp
tglRasterizeSeg(greenLine);
```
Finally, to draw it to the window, use `tglDrawSeg()` and pass the `TGLseg` object and `TGLwindow` surface to draw it to.
```cpp
tglDrawSeg(greenLine, SCR);
```
