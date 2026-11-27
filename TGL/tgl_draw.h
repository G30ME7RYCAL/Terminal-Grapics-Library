#pragma once

#include <iostream>
#include <algorithm>
#include <string>

#include "tgl_styles.h"
#include "tgl_geometry.h"
#include "tgl_log.h"

// Transfers the matrix of a TGLrect object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawRect(TGLrect &rect, TGLwindow &window) {
    if (rect.TGL_Rect_Width + rect.TGL_Rect_X > window.TGL_Window_Width ||
        rect.TGL_Rect_Height + rect.TGL_Rect_Y > window.TGL_Window_Height ||
        rect.TGL_Rect_X < 0 ||
        rect.TGL_Rect_Y < 0) {
        pushErr("Rectangle goes out of bounds.");
        return false;
    }
    

    if (rect.TGL_Rect_Width < 0 ||
        rect.TGL_Rect_Height < 0) {
        pushErr("Rectangles cannot have negative dimensions.");
        return false;
    }


    TGLstyle rectFill {rect.TGL_Fill_Color, rect.TGL_Fill_Glyph};

    for (int row = rect.TGL_Rect_Y; row < rect.TGL_Rect_Height + rect.TGL_Rect_Y; row++) {
        for (int col = rect.TGL_Rect_X; col < rect.TGL_Rect_Width + rect.TGL_Rect_X; col++) {
            tglSetStyle(window.TGL_Window_Vect[row][col], rectFill);
        }
    }
    return true;
}

// Tranfers a TGLpoint object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawPoint(TGLpoint &point, TGLwindow &window) {
    if (point.x >= window.TGL_Window_Width ||
        point.y >= window.TGL_Window_Height ||
        point.x < 0 ||
        point.y < 0) {
        pushErr("TGLpoint object out of range of the window.");
        return false;
    }
    
    TGLstyle pointStyle {point.TGL_Point_Color, point.TGL_Point_Glyph};

    tglSetStyle(window.TGL_Window_Vect[point.y][point.x], pointStyle);
    return true;
}

// Transfers the vector of points (std::vector<TGLpoint>) of a TGLsegment object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawSeg(TGLseg &seg, TGLwindow &window) {
    for (TGLpoint &point : seg.points) {
        tglDrawPoint(point, window);
    }
    return true;
}

// Transfers the vector of edges (std::vector<TGLseg>) of a TGLpolyFrame object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawPolyFrame(TGLpolyFrame &frame, TGLwindow &window) {
    for (TGLseg &seg : frame.sides) {
        tglDrawSeg(seg, window);
    }
    return true;
}

inline bool tglDrawEllipseFrame(TGLellipseFrame &frame, TGLwindow &window) {
    for (TGLpoint &point : frame.edge) {
        tglDrawPoint(point, window);
    }
    return true;
}

// Transfers a TGLpolygon object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawPolygon(TGLpolygon &poly, TGLwindow &window, bool drawFrame = true) {
    for (TGLpoint &point : poly.fill) {
        tglDrawPoint(point, window);
    }

    if (drawFrame == true) {
        tglDrawPolyFrame(poly.frame, window);
    }

    return true;
}

// Transfers a TGLellipse object to a TGLwindow object. Returns boolean success status.
inline bool tglDrawEllipse(TGLellipse &ellipse, TGLwindow &window, bool drawFrame = true) {
    for (TGLpoint &point : ellipse.fill) {
        tglDrawPoint(point, window);
    }

    if (drawFrame == true) {
        tglDrawEllipseFrame(ellipse.frame, window);
    }

    return true;
}