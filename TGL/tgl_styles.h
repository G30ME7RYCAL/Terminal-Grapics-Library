#pragma once

#include <string>

#include "TSL/tsl.h"

//A color data type that holds an RGB
using TGLcolor = float4;

// Combined style holding a glyph and TGLcolor
struct TGLstyle {
    TGLcolor color;
    char glyph = ' ';
};

//Converts 8-bit RGB values into floating-point numbers between 0 and 1.
inline TGLcolor RGB8(int r, int g, int b, int a = 0xFF) {
    return {r/255.f, g/255.f, b/255.f, a/255.f};
}

const inline std::string TGL_RESET = "\033[0m";
const inline TGLcolor TGL_BLACK = RGB8(0, 0, 0);
const inline TGLcolor TGL_WHITE = RGB8(0xFF, 0xFF, 0xFF);
const inline TGLcolor TGL_GRAY = RGB8(0x88, 0x88, 0x88);
const inline TGLcolor TGL_RED = RGB8(0xFF, 0, 0);
const inline TGLcolor TGL_ORANGE = RGB8(0xFF, 0x88, 0);
const inline TGLcolor TGL_YELLOW = RGB8(0xFF, 0xFF, 0);
const inline TGLcolor TGL_DARK_GREEN = RGB8(0, 0x88, 0);
const inline TGLcolor TGL_GREEN = RGB8(0, 0xFF, 0);
const inline TGLcolor TGL_BLUE = RGB8(0, 0, 0xFF);
const inline TGLcolor TGL_DARK_BLUE = RGB8(0, 0, 0x88);
const inline TGLcolor TGL_CYAN = RGB8(0, 0xFF, 0xFF);
const inline TGLcolor TGL_INDIGO = RGB8(0x44, 0, 0xFF);
const inline TGLcolor TGL_PURPLE = RGB8(0x88, 0, 0xFF);
const inline TGLcolor TGL_MAGENTA = RGB8(0xFF, 0, 0xFF);

const inline TGLcolor TGL_CUSTOM(float r, float g, float b, float a = 1.0f) {
    return {r, g, b, a};
}

const inline TGLcolor TGL_CUSTOM_8(int r, int g, int b, int a = 0xFF) {
    return RGB8(r, g, b, a);
}

inline int tglToBinary(float v) {
    return (int)(std::min(std::max(v, 0.0f), 1.0f) * 255.0f + 0.5f);
}

//Returns the color of a specified loaded into an std::string
inline std::string loadColor(const TGLcolor &color) {
    return "\033[48;2;" + std::to_string(tglToBinary(color.x)) + ";"
                        + std::to_string(tglToBinary(color.y)) + ";"
                        + std::to_string(tglToBinary(color.z)) + "m";
}

//Sets one style to another style and blends according to transparency.
inline void tglSetStyle(TGLstyle &dst, TGLstyle src) {
    dst.color = lerp(dst.color, src.color, src.color.w);
    dst.glyph = src.glyph;
}