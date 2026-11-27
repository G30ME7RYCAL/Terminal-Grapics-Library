#pragma once

#include <iostream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "tgl_styles.h"
#include "tgl_constants.h"
#include "tgl_draw.h"
#include "tgl_geometry.h"
#include "tgl_log.h"

#include "TSL/tsl.h"

//=============================================================================
//-  - -- ----------=--=--==-===   TGL Shaders   ===-==--=--=---------- -- -  -
//=============================================================================

// Changes the fill of any filled TGL shape according to a specified fragment shader. Returns boolean success status. Programmable with C++.
template <typename TGLshape, typename TGLshader>
inline bool tglFragmentShader(TGLshape &shape, TGLshader shader) {
    for (TGLpoint &point : shape.fill) {
        point.TGL_Point_Color = shader(point, shape);
    }
    return true;
}

//=============================================================================
//-  - -- ---------=--=--==-===   TGL Functions   ===-==--=--=--------- -- -  -
//=============================================================================

//Resets styles, clears error log and flushes terminal. Returns boolean success status.
inline bool tglInit() {
    errLog = "";

    std::cout << TGL_RESET; // Reset terminal style
    std::cout << "\033[2J"; // Clear terminal screen
    std::cout << "\033[H";// Move cursor to top left
    std::cout.flush();
    std::cout << std::endl;

    return true;
}

//Clears the terminal screen without resetting styles
inline bool tglClearTerminal() {
    std::cout << "\033[2J"; // Clear terminal screen
    std::cout << "\033[H";// Move cursor to top left
    return true;
}

//Returns a double for how many seconds the program has been running with millisecond precision.
inline double tglGetTime() {
    static const auto start = std::chrono::steady_clock::now();

    const auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() / 1000.0;
}


// ---------------------------
//   2D Object Rasterization
// ---------------------------

//Initializes the matrix of a TGLwindow object. Returns boolean success status.
inline bool tglCreateWindow(TGLwindow &window) {
    if (window.TGL_Window_Height < 0) {
        pushErr("A window cannot have negative height.");
        return false;
    }
    if (window.TGL_Window_Width < 0) {
        pushErr("A window cannot have negative width.");
        return false;
    }

    TGLstyle windowStyle {window.TGL_Window_Color, window.TGL_Window_Glyph};

    window.TGL_Window_Vect = std::vector<std::vector<TGLstyle>>(
       window.TGL_Window_Height,
       std::vector<TGLstyle>(window.TGL_Window_Width, windowStyle)
   );
   return true;
}

//Initializes the matrix of a TGLrect object. Returns boolean success status.
inline bool tglRasterizeRect(TGLrect &rect) {
    if (rect.TGL_Rect_Height < 0) {
        pushErr("A rectangle cannot have negative height.");
        return false;
    }
    if (rect.TGL_Rect_Width < 0) {
        pushErr("A regtangle cannot have negative width.");
        return false;
    }

    TGLstyle rectFill {rect.TGL_Fill_Color, rect.TGL_Fill_Glyph};

    rect.TGL_Rect_Vect = std::vector<std::vector<TGLstyle>>(
       rect.TGL_Rect_Height,
       std::vector<TGLstyle>(rect.TGL_Rect_Width, rectFill)
   );
   return true;
}

//Initializes a vector of points (std::vector<TGLpoint>) of a TGLseg object using Bresenham's line algorithm. Returns boolean success status.
inline bool tglRasterizeSeg(TGLseg &seg) {
    seg.points.clear();

    int x1 = seg.a.x;
    int x2 = seg.b.x;
    int y1 = seg.a.y;
    int y2 = seg.b.y;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        seg.points.push_back(
            TGLpoint {
                x1, y1,
                seg.TGL_Border_Color,
                seg.TGL_Border_Glyph
            }
        );

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = err * 2;
        
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    return true;
}

//Initializes a vector of edges (std::vector<TGLseg>) of a TGLpolyFrame object. Returns boolean success status.
inline bool tglRasterizePolyFrame(TGLpolyFrame &frame) {
    frame.sides.clear();

    if (frame.vertices.size() < 3) {
        pushErr("A polyFrame must have at least 3 vertices");
        return false;
    }
    
    for (std::size_t point = 0; point < frame.vertices.size(); point++) {
        TGLpoint p1 = frame.vertices[point];
        TGLpoint p2 = frame.vertices[(point+1) % frame.vertices.size()];

        TGLseg side {
            p1,
            p2,
            frame.TGL_Border_Color,
            frame.TGL_Border_Glyph
        };

        tglRasterizeSeg(side);

        // Remove duplicate points
        if (!side.points.empty()) {
            side.points.erase(side.points.begin());
        }

        frame.sides.push_back(side);
    }

    return true;
}

//Initializes a vector of border points (std::vector<TGLpoint>) of a TGLellipseFrame object. Returns boolean success status.
inline bool tglRasterizeEllipseFrame(TGLellipseFrame &frame) {
    frame.edge.clear();

    // Make shorter names for the foci
    TGLpoint f1 = frame.f1;
    TGLpoint f2 = frame.f2;
    double L = frame.majorAxis;

    if (f1.x < 0 || f1.y < 0 ||
        f2.x < 0 || f2.y < 0) {
        pushErr("Foci cannot have negative coordinates.");
        return false;
    }

    if (L < std::sqrt(std::pow(f1.x - f2.x, 2) + std::pow(f1.y - f2.y, 2))) {
        pushErr("The length major axis must be greater than the distance between the two foci.");
        return false;
    }

    // Define bounding box
    int minX = static_cast<int>((f1.x + f2.x - std::sqrt(L*L - (f1.y-f2.y)*(f1.y-f2.y))) / 2.0);
    int maxX = static_cast<int>((f1.x + f2.x + std::sqrt(L*L - (f1.y-f2.y)*(f1.y-f2.y))) / 2.0);
    int minY = static_cast<int>((f1.y + f2.y - std::sqrt(L*L - (f1.x-f2.x)*(f1.x-f2.x))) / 2.0);
    int maxY = static_cast<int>((f1.y + f2.y + std::sqrt(L*L - (f1.x-f2.x)*(f1.x-f2.x))) / 2.0);

    for (int pointX = minX; pointX <= maxX; pointX++) {
        for (int pointY = minY; pointY <= maxY; pointY++) {
            TGLpoint currentPoint {pointX, pointY, frame.TGL_Border_Color, frame.TGL_Border_Glyph};

            if (tglOnEllipseFrame(currentPoint, frame)) {
                frame.edge.push_back(currentPoint);
            }
        }
    }

    return true;
}

//Initializes a border (TGLpolyFrame) and a vector of fill points (std::vector<TGLpoint>) for a TGLpolygon object. Returns boolean success status.
inline bool tglRasterizePolygon(TGLpolygon &poly, bool excludeBorder = false) {
    if (poly.vertices.size() < 3) {
        pushErr("A polygon must have at least 3 vertices");
        return false;
    }

    poly.fill.clear();

    poly.frame = {
        poly.vertices,
        poly.TGL_Border_Color,
        poly.TGL_Border_Glyph
    };

    if (!tglRasterizePolyFrame(poly.frame)) {
        pushErr("Error Creating Polygon Frame");
        return false;
    }

    // Create bounding box
    auto [leftIt, rightIt] = std::minmax_element(
        poly.vertices.begin(),
        poly.vertices.end(),
        [](const TGLpoint &a, const TGLpoint &b) {
            return a.x < b.x;
        }
    );

    auto [lowerIt, upperIt] = std::minmax_element(
        poly.vertices.begin(),
        poly.vertices.end(),
        [](const TGLpoint &a, const TGLpoint &b) {
            return a.y < b.y;
        }
    );

    int minX = leftIt->x;
    int maxX = rightIt->x;
    int minY = lowerIt->y;
    int maxY = upperIt->y;

    int boundW = maxX - minX + 1;
    int boundH = maxY - minY + 1;
    std::vector<std::vector<bool>> onBorder(boundW, std::vector<bool>(boundH, false));

    if (excludeBorder) {
        for(const TGLseg &seg : poly.frame.sides) {
            for(const TGLpoint &point : seg.points) {
                onBorder[point.x - minX][point.y - minY] = true;
            }
        }
    }

    // Populate poly.fill
    for (int pointX = minX; pointX <= maxX; pointX++) {
        for (int pointY = minY; pointY <= maxY; pointY++) {
            if (onBorder[pointX - minX][pointY - minY]) { continue; }

            TGLpoint currentPoint {pointX, pointY, poly.TGL_Fill_Color, poly.TGL_Fill_Glyph};
            
            if (tglPointInPolygon(currentPoint, poly)) {
                poly.fill.push_back(currentPoint);
            }
        }
    }

    return true;
}

//Initializes a border (TGLellipseFrame) and a vector of fill points (std::vector<TGLpoint>) for a TGLellipse object. Returns boolean success status.
inline bool tglRasterizeEllipse(TGLellipse &ellipse, bool excludeBorder = false) {
    // Make shorter names for the foci
    TGLpoint f1 = ellipse.f1;
    TGLpoint f2 = ellipse.f2;
    double L = ellipse.majorAxis;

    //Check invalid input
    if (f1.x < 0 || f1.y < 0 ||
        f2.x < 0 || f2.y < 0) {
        pushErr("Foci cannot have negative coordinates.");
        return false;
    }

    if (L < std::sqrt(std::pow(f1.x - f2.x, 2) + std::pow(f1.y - f2.y, 2))) {
        pushErr("The length major axis must be greater than the distance between the two foci.");
        return false;
    }

    ellipse.fill.clear();

    ellipse.frame = {f1, f2, L,
                     ellipse.TGL_Border_Color,
                     ellipse.TGL_Border_Glyph
                    };
    
    if (!tglRasterizeEllipseFrame(ellipse.frame)) {
        pushErr("Error Creating Ellipse Frame");
        return false;
    }

    // Define bounding box
    int minX = static_cast<int>((f1.x + f2.x - std::sqrt(L*L - (f1.y-f2.y)*(f1.y-f2.y))) / 2.0);
    int maxX = static_cast<int>((f1.x + f2.x + std::sqrt(L*L - (f1.y-f2.y)*(f1.y-f2.y))) / 2.0);
    int minY = static_cast<int>((f1.y + f2.y - std::sqrt(L*L - (f1.x-f2.x)*(f1.x-f2.x))) / 2.0);
    int maxY = static_cast<int>((f1.y + f2.y + std::sqrt(L*L - (f1.x-f2.x)*(f1.x-f2.x))) / 2.0);

    for (int pointX = minX; pointX <= maxX; pointX++) {
        for (int pointY = minY; pointY <= maxY; pointY++) {
            TGLpoint currentPoint {pointX, pointY, ellipse.TGL_Fill_Color, ellipse.TGL_Fill_Glyph};

            if (tglInEllipse(currentPoint, ellipse, excludeBorder)) {
                ellipse.fill.push_back(currentPoint);
            }
        }
    }

    return true;
}

//Returns whether a file has a specified extension.
inline bool hasExtension(const std::string &path, const std::string &ext) {
    if (path.size() < ext.size()) return false;
    std::string end = path.substr(path.size() - ext.size());
    std::transform(end.begin(), end.end(), end.begin(), tolower); //make it case-insensitive
    return end == ext;
}

//Converts a 0.0 - 1.0 double to a byte (0 - 255)
inline uint8_t tglToByte(float v) {
    return static_cast<uint8_t>(std::min(std::max(v, 0.0f), 1.0f) * 255.0f + 0.5f);
}

//Writes a window to a PPM file rather than the terminal. Returns boolean success status.
inline bool tglWritePPM(const TGLwindow &window, const std::string &path) {
    if (!hasExtension(path, ".ppm")) {
        pushErr("target file path must have .ppm file extension");
        return false;
    }

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        pushErr("could not initialize rasterization target");
        return false;
    }

    out << "P6\n" << window.TGL_Window_Width << " " << window.TGL_Window_Height << "\n255\n";

    std::string pixels;
    pixels.reserve(window.TGL_Window_Width * window.TGL_Window_Height * 3);

    for (int row = 0; row < window.TGL_Window_Height; row++) {
        for (int col = 0; col < window.TGL_Window_Width; col++) {
            const TGLcolor &c = window.TGL_Window_Vect[row][col].color;
            pixels += static_cast<char>(tglToByte(c.x)); //red
            pixels += static_cast<char>(tglToByte(c.y)); //green
            pixels += static_cast<char>(tglToByte(c.z)); //blue
        }
    }

    out.write(pixels.data(), pixels.size());
    return out.good();
}

//Writes a window to a BMP file rather than the terminal. Returns boolean success status.
inline bool tglWriteBMP(const TGLwindow &window, const std::string &path) {
    if (!hasExtension(path, ".bmp")) {
        pushErr("target file path must have .bmp file extension");
        return false;
    }

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        pushErr("could not initialize rasterization target");
        return false;
    }

    const int rowPadded = (window.TGL_Window_Width * 3 + 3) & (~3); // BMP rows must align to 4 bytes
    const int padSize = rowPadded - (window.TGL_Window_Width * 3);
    const int fileSize = 54 + rowPadded * window.TGL_Window_Height;

    uint8_t header[54] = {
        'B','M',  0,0,0,0,  0,0, 0,0,  54,0,0,0, // File header
        40,0,0,0, 0,0,0,0,  0,0,0,0, 1,0, 24,0   // Info header (24-bit RGB)
    };

    auto writeDWord = [&](int offset, int32_t val) {
        header[offset]     = static_cast<uint8_t>(val & 0xFF);
        header[offset + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
        header[offset + 2] = static_cast<uint8_t>((val >> 16) & 0xFF);
        header[offset + 3] = static_cast<uint8_t>((val >> 24) & 0xFF);
    };

    writeDWord(2, fileSize);
    writeDWord(18, window.TGL_Window_Width);
    writeDWord(22, -window.TGL_Window_Height);


    std::string pixels;
    pixels.reserve(fileSize);
    pixels.append(reinterpret_cast<const char*>(header), 54);

    for (int row = 0; row < window.TGL_Window_Height; row++) {
        for (int col = 0; col < window.TGL_Window_Width; col++) {
            const TGLcolor &c = window.TGL_Window_Vect[row][col].color;
            pixels += static_cast<char>(tglToByte(c.z)); //blue
            pixels += static_cast<char>(tglToByte(c.y)); //green
            pixels += static_cast<char>(tglToByte(c.x)); //red
        }
        
        // Pad end of row to a multiple of 4 bytes
        for (int p = 0; p < padSize; p++) {
            pixels += '\0';
        }
    }

    out.write(pixels.data(), pixels.size());
    return out.good();
}

// Loads a TGLwindow object to a target (set by default to terminal). Returns boolean success status.
inline bool tglUpdateDisplay(const TGLwindow &window, const std::string &target = "") {
    if (hasExtension(target, ".ppm")) {
        return tglWritePPM(window, target);
    }
        if (hasExtension(target, ".bmp")) {
        return tglWriteBMP(window, target);
    }

    std::string buffer;
    buffer.reserve(window.TGL_Window_Width * window.TGL_Window_Height * 10); //multiply by 10 for safety and performance (bare minimum is 2)
    buffer += "\033[?2026h"; //start synchronized update
    buffer += "\033[H";      //home position
    
    for (int row = 0; row < window.TGL_Window_Height; row++) {
        TGLcolor lastColor {-1, -1, -1, -1};

        for (int col=0; col < window.TGL_Window_Width; col++) {
            const TGLstyle &style = window.TGL_Window_Vect[row][col];

            TGLcolor color = style.color;

            if (color != lastColor) { //only need to output a new color if it differs from the last pixels color
                buffer += loadColor(style.color);
                lastColor = color;
            }
            buffer += ' ';
            buffer += style.glyph;
        }
        buffer += TGL_RESET;
        buffer += "\n"; //go to next row of pixels
    }

    buffer += "\033[?2026l"; //end synchronized update

    std::cout << buffer;
    std::cout.flush();

    return true;
}