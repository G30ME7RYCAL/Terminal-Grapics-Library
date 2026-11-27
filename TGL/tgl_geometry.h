#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "tgl_styles.h"
#include "tgl_constants.h"
#include "tgl_log.h"

//A data type that holds a window's dimensions and style.
struct TGLwindow {
   int TGL_Window_Width;
   int TGL_Window_Height;
   TGLcolor TGL_Window_Color = TGL_BLACK;
   char TGL_Window_Glyph = ' ';

   std::vector<std::vector<TGLstyle>> TGL_Window_Vect;
};

//A data type that holds a rectangles's dimensions, topleft coordinates, and fill style.
struct TGLrect {
   int TGL_Rect_Width;
   int TGL_Rect_Height;
   int TGL_Rect_X;
   int TGL_Rect_Y;
   TGLcolor TGL_Fill_Color;
   char TGL_Fill_Glyph = ' ';

   std::vector<std::vector<TGLstyle>> TGL_Rect_Vect;
};

//A data type that holds a point's coordinates and point style.
struct TGLpoint {
    int x;
    int y;
    TGLcolor TGL_Point_Color;
    char TGL_Point_Glyph = ' ';

    bool operator==(const TGLpoint &other) const {
        return x == other.x && y == other.y;
    }
};

//A data type that holds a segment's start point (TGLpoint), end point (TGLpoint), and border style.
struct TGLseg {
    TGLpoint a;
    TGLpoint b;
    TGLcolor TGL_Border_Color;
    char TGL_Border_Glyph = ' ';

    std::vector<TGLpoint> points; 
};

//A data type that holds the outline of a polygon based on a vector of vertices (std::vector<TGLpoint>) and border style.
struct TGLpolyFrame {
    std::vector<TGLpoint> vertices;
    TGLcolor TGL_Border_Color = TGL_BLACK;
    char TGL_Border_Glyph = ' ';

    std::vector<TGLseg> sides;
};

//A data type that holds the outline of an ellipse determined by 2 foci (TGLpoint), the length of it's major axis, and a border style.
struct TGLellipseFrame {
    TGLpoint f1;
    TGLpoint f2;
    double majorAxis;
    TGLcolor TGL_Border_Color;
    char TGL_Border_Glyph = ' ';

    std::vector<TGLpoint> edge;
};

//A data type that holds a polygon based on a vector of vertices (std::vector<TGLpoint>), fill style, and border style.
struct TGLpolygon {
    std::vector<TGLpoint> vertices;
    TGLcolor TGL_Fill_Color = TGL_BLACK;
    TGLcolor TGL_Border_Color = TGL_Fill_Color;
    char TGL_Fill_Glyph = ' ';
    char TGL_Border_Glyph = ' ';

    std::vector<TGLpoint> fill;
    TGLpolyFrame frame;
};

//A data type that holds an ellipse determined by 2 foci (TGLpoint), the length of it's major axis, fill style, and border style.
struct TGLellipse {
    TGLpoint f1;
    TGLpoint f2;
    double majorAxis;
    TGLcolor TGL_Fill_Color = TGL_BLACK;
    TGLcolor TGL_Border_Color = TGL_Fill_Color;
    char TGL_Fill_Glyph = ' ';
    char TGL_Border_Glyph = ' ';

    std::vector<TGLpoint> fill;
    TGLellipseFrame frame;
};

//===========================================================================
//-  - -- -----------=--=--==-===   Helpers   ===-==--=--=----------- -- -  -
//===========================================================================

// Converts degrees to radians.
inline double tglToRadians(double degrees) {
    return degrees * (PI / 180);
}

//Returns true if a point (TGLpoint) is on a segment. (TGLseg)
inline bool tglPointOnSeg(const TGLpoint &point, const TGLseg &seg) {
    int intersects = std::count(seg.points.begin(), seg.points.end(), point);
    if (intersects > 0) {
        return true;
    } else {
        return false;
    }
}

//Returns a vector of all rasterized points (std::vector<TGLpoint>) where 2 segments (TGLseg) intersect.
inline std::vector<TGLpoint> tglSegIntersections(const TGLseg &seg1, const TGLseg &seg2) {
    std::vector<TGLpoint> intersections = {};

    for (const auto &point : seg1.points) {
        if (tglPointOnSeg(point, seg2)) {
            intersections.push_back(point);
        }
    }
    return intersections;
}

//Returns true if a point (TGLpoint) is inside a polygon. (TGLpolygon).
inline bool tglPointInPolygon(const TGLpoint &point, const TGLpolygon &poly) {
    if (poly.vertices.size() < 3) {
        pushErr("A polygon must have at least 3 vertices.");
        return false;
    }

    bool inside = false;

    std::size_t prevVertex = poly.vertices.size() - 1;
    
    for (std::size_t currVertex = 0; currVertex < poly.vertices.size(); currVertex++) {
        const TGLpoint &a = poly.vertices[currVertex];
        const TGLpoint &b = poly.vertices[prevVertex];

        if ((a.y > point.y) != (b.y > point.y)) {

            double intersectionX = a.x + (double)(b.x - a.x) * (point.y - a.y) / (b.y - a.y);

            if (point.x < intersectionX) {
                inside = !inside;
            }
        }

        prevVertex = currVertex;
    }

    return inside;
}

//Calculates the distance between 2 points. (TGLpoint).
inline double tglPointDist(TGLpoint &p1, TGLpoint &p2) {
    int distX = std::abs(p1.x - p2.x);
    int distY = std::abs(p1.y - p2.y);
    return std::sqrt(distX*distX + distY*distY);
}

//Returns true if a point (TGLpoint) is along an ellipse's frame (TGLellipseFrame).
inline bool tglOnEllipseFrame(TGLpoint &point, TGLellipseFrame &frame) {
    TGLpoint f1 = frame.f1;
    TGLpoint f2 = frame.f2;

    double pointDistance = tglPointDist(f1, point) + tglPointDist(f2, point);

    if (pointDistance > frame.majorAxis) { return false; }

    std::vector<int> dx {-1, 1, 0, 0};
    std::vector<int> dy {0, 0, -1, 1};
    for (int i = 0; i < 4; i++) {
        TGLpoint neighbor {point.x + dx[i], point.y + dy[i]};

        if (tglPointDist(f1, neighbor) + tglPointDist(f2, neighbor) > frame.majorAxis) {
            return true;
        }
    }
    return false;
}

//Returns true if a point (TGLpoint) is inside an ellipse. (TGLellipse).
inline bool tglInEllipse(TGLpoint &point, TGLellipse &ellipse, bool excludeBorder = false) {
    if(excludeBorder) {
        if (tglOnEllipseFrame(point, ellipse.frame)) {
            return false;
        }
    }

    if (tglPointDist(ellipse.f1, point) + tglPointDist(ellipse.f2, point) < ellipse.majorAxis) {
        return true;
    }
    
    return false;
}