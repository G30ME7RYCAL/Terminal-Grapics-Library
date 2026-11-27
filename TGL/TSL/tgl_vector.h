#pragma once

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

//============================================================================
//-  - -- -----------=--=--==-===  2D Vectors  ===-==--=--=----------- -- -  -
//============================================================================

template <typename T>
struct vec2 {
    T x = T(0), y = T(0);

    // Constructors
    constexpr vec2() = default;
    constexpr vec2(T x, T y) : x(x), y(y) {}
    constexpr explicit vec2(T v) : x(v), y(v) {}

    // Comparison
    constexpr bool operator==(const vec2 &o) const {
        return x == o.x && y == o.y;
    }
    constexpr bool operator!=(const vec2 &o) const {
        return x != o.x || y != o.y;
    }

    // Unary Operations
    constexpr vec2 operator+() const { return *this; }
    constexpr vec2 operator-() const { return { -x, -y }; }

    constexpr vec2 &operator++() {
        static_assert(!std::is_same_v<T, bool>, "Cannot increment bool");
        ++x; ++y;
        return *this;
    }
    constexpr vec2 &operator--() {
        static_assert(!std::is_same_v<T, bool>, "Cannot decrement bool");
        --x; --y;
        return *this;
    }
    constexpr vec2 operator++(int) {
        vec2 old = *this;
        ++(*this);
        return old;
    }
    constexpr vec2 operator--(int) {
        vec2 old = *this;
        --(*this);
        return old;
    }

    // Vector-Vector arithmetic
    constexpr vec2 operator+(const vec2 &o) const { return { x + o.x, y + o.y }; }
    constexpr vec2 operator-(const vec2 &o) const { return { x - o.x, y - o.y }; }
    constexpr vec2 operator*(const vec2 &o) const { return { x * o.x, y * o.y }; }
    constexpr vec2 operator/(const vec2 &o) const { return { x / o.x, y / o.y }; }

    // Vector-Scalar arithmetic
    constexpr vec2 operator+(T s) const { return { x + s, y + s }; }
    constexpr vec2 operator-(T s) const { return { x - s, y - s }; }
    constexpr vec2 operator*(T s) const { return { x * s, y * s }; }
    constexpr vec2 operator/(T s) const { return { x / s, y / s }; }

    // Compound assignments (Vector)
    constexpr vec2 &operator+=(const vec2 &o) { x += o.x; y += o.y; return *this; }
    constexpr vec2 &operator-=(const vec2 &o) { x -= o.x; y -= o.y; return *this; }
    constexpr vec2 &operator*=(const vec2 &o) { x *= o.x; y *= o.y; return *this; }
    constexpr vec2 &operator/=(const vec2 &o) { x /= o.x; y /= o.y; return *this; }

    // Compound assignments (Scalar)
    constexpr vec2 &operator+=(T s) { x += s; y += s; return *this; }
    constexpr vec2 &operator-=(T s) { x -= s; y -= s; return *this; }
    constexpr vec2 &operator*=(T s) { x *= s; y *= s; return *this; }
    constexpr vec2 &operator/=(T s) { x /= s; y /= s; return *this; }

    // Index access
    constexpr T &operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        throw std::out_of_range("2D vector index must be 0 or 1");
    }
    constexpr const T &operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        throw std::out_of_range("2D vector index must be 0 or 1");
    }
};


//============================================================================
//-  - -- -----------=--=--==-===  3D Vectors  ===-==--=--=----------- -- -  -
//============================================================================

template <typename T>
struct vec3 {
    T x = T(0), y = T(0), z = T(0);

    // Constructors
    constexpr vec3() = default;
    constexpr vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    constexpr explicit vec3(T v) : x(v), y(v), z(v) {}
    constexpr vec3(const vec2<T> &xy, T z) : x(xy.x), y(xy.y), z(z) {}
    constexpr vec3(T x, const vec2<T> &yz) : x(x), y(yz.x), z(yz.y) {}

    // Comparison
    constexpr bool operator==(const vec3 &o) const {
        return x == o.x && y == o.y && z == o.z;
    }
    constexpr bool operator!=(const vec3 &o) const {
        return x != o.x || y != o.y || z != o.z;
    }

    // Unary Operations
    constexpr vec3 operator+() const { return *this; }
    constexpr vec3 operator-() const { return { -x, -y, -z }; }

    constexpr vec3 &operator++() {
        static_assert(!std::is_same_v<T, bool>, "Cannot increment bool");
        ++x; ++y; ++z;
        return *this;
    }
    constexpr vec3 &operator--() {
        static_assert(!std::is_same_v<T, bool>, "Cannot decrement bool");
        --x; --y; --z;
        return *this;
    }
    constexpr vec3 operator++(int) { vec3 old = *this; ++(*this); return old; }
    constexpr vec3 operator--(int) { vec3 old = *this; --(*this); return old; }

    // Vector-Vector arithmetic
    constexpr vec3 operator+(const vec3 &o) const { return { x + o.x, y + o.y, z + o.z }; }
    constexpr vec3 operator-(const vec3 &o) const { return { x - o.x, y - o.y, z - o.z }; }
    constexpr vec3 operator*(const vec3 &o) const { return { x * o.x, y * o.y, z * o.z }; }
    constexpr vec3 operator/(const vec3 &o) const { return { x / o.x, y / o.y, z / o.z }; }

    // Vector-Scalar arithmetic
    constexpr vec3 operator+(T s) const { return { x + s, y + s, z + s }; }
    constexpr vec3 operator-(T s) const { return { x - s, y - s, z - s }; }
    constexpr vec3 operator*(T s) const { return { x * s, y * s, z * s }; }
    constexpr vec3 operator/(T s) const { return { x / s, y / s, z / s }; }

    // Compound assignments (Vector)
    constexpr vec3 &operator+=(const vec3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    constexpr vec3 &operator-=(const vec3 &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    constexpr vec3 &operator*=(const vec3 &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    constexpr vec3 &operator/=(const vec3 &o) { x /= o.x; y /= o.y; z /= o.z; return *this; }

    // Compound assignments (Scalar)
    constexpr vec3 &operator+=(T s) { x += s; y += s; z += s; return *this; }
    constexpr vec3 &operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
    constexpr vec3 &operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
    constexpr vec3 &operator/=(T s) { x /= s; y /= s; z /= s; return *this; }

    // Index access
    constexpr T &operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        throw std::out_of_range("3D vector index must be 0, 1, or 2");
    }
    constexpr const T &operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        throw std::out_of_range("3D vector index must be 0, 1, or 2");
    }
};


//============================================================================
//-  - -- -----------=--=--==-===  4D Vectors  ===-==--=--=----------- -- -  -
//============================================================================

template <typename T>
struct vec4 {
    T x = T(0), y = T(0), z = T(0), w = T(0);

    // Constructors
    constexpr vec4() = default;
    constexpr vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    constexpr explicit vec4(T v) : x(v), y(v), z(v), w(v) {}
    constexpr vec4(const vec2<T> &xy, T z, T w) : x(xy.x), y(xy.y), z(z), w(w) {}
    constexpr vec4(T x, T y, const vec2<T> &zw) : x(x), y(y), z(zw.x), w(zw.y) {}
    constexpr vec4(T x, const vec2<T> &yz, T w) : x(x), y(yz.x), z(yz.y), w(w) {}
    constexpr vec4(const vec3<T> &xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    constexpr vec4(T x, const vec3<T> &yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
    constexpr vec4(const vec2<T> &xy, const vec2<T> &zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

    // Comparison
    constexpr bool operator==(const vec4 &o) const {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }
    constexpr bool operator!=(const vec4 &o) const {
        return x != o.x || y != o.y || z != o.z || w != o.w;
    }

    // Unary Operations
    constexpr vec4 operator+() const { return *this; }
    constexpr vec4 operator-() const { return { -x, -y, -z, -w }; }

    constexpr vec4 &operator++() {
        static_assert(!std::is_same_v<T, bool>, "Cannot increment bool");
        ++x; ++y; ++z; ++w;
        return *this;
    }
    constexpr vec4 &operator--() {
        static_assert(!std::is_same_v<T, bool>, "Cannot decrement bool");
        --x; --y; --z; --w;
        return *this;
    }
    constexpr vec4 operator++(int) { vec4 old = *this; ++(*this); return old; }
    constexpr vec4 operator--(int) { vec4 old = *this; --(*this); return old; }

    // Vector-Vector arithmetic
    constexpr vec4 operator+(const vec4 &o) const { return { x + o.x, y + o.y, z + o.z, w + o.w }; }
    constexpr vec4 operator-(const vec4 &o) const { return { x - o.x, y - o.y, z - o.z, w - o.w }; }
    constexpr vec4 operator*(const vec4 &o) const { return { x * o.x, y * o.y, z * o.z, w * o.w }; }
    constexpr vec4 operator/(const vec4 &o) const { return { x / o.x, y / o.y, z / o.z, w / o.w }; }

    // Vector-Scalar arithmetic
    constexpr vec4 operator+(T s) const { return { x + s, y + s, z + s, w + s }; }
    constexpr vec4 operator-(T s) const { return { x - s, y - s, z - s, w - s }; }
    constexpr vec4 operator*(T s) const { return { x * s, y * s, z * s, w * s }; }
    constexpr vec4 operator/(T s) const { return { x / s, y / s, z / s, w / s }; }

    // Compound assignments (Vector)
    constexpr vec4 &operator+=(const vec4 &o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
    constexpr vec4 &operator-=(const vec4 &o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
    constexpr vec4 &operator*=(const vec4 &o) { x *= o.x; y *= o.y; z *= o.z; w *= o.w; return *this; }
    constexpr vec4 &operator/=(const vec4 &o) { x /= o.x; y /= o.y; z /= o.z; w /= o.w; return *this; }

    // Compound assignments (Scalar)
    constexpr vec4 &operator+=(T s) { x += s; y += s; z += s; w += s; return *this; }
    constexpr vec4 &operator-=(T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
    constexpr vec4 &operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    constexpr vec4 &operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }

    // Index access
    constexpr T &operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        throw std::out_of_range("4D vector index must be 0, 1, 2, or 3");
    }
    constexpr const T &operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;
        throw std::out_of_range("4D vector index must be 0, 1, 2, or 3");
    }
};


//============================================================================
//-  - -- -----=--=--==-=== Scalar-Vector Arithmetic ===-==--=--=----- -- -  -
//============================================================================

template <typename T>
constexpr vec2<T> operator+(T s, const vec2<T> &v) {
    return { s + v.x, s + v.y };
}
template <typename T>
constexpr vec2<T> operator-(T s, const vec2<T> &v) {
    return { s - v.x, s - v.y };
}
template <typename T>
constexpr vec2<T> operator*(T s, const vec2<T> &v) {
    return { s * v.x, s * v.y };
}
template <typename T>
constexpr vec2<T> operator/(T s, const vec2<T> &v) {
    return { s / v.x, s / v.y };
}


template <typename T>
constexpr vec3<T> operator+(T s, const vec3<T> &v) {
    return { s + v.x, s + v.y, s + v.z };
}
template <typename T>
constexpr vec3<T> operator-(T s, const vec3<T> &v) {
    return { s - v.x, s - v.y, s - v.z };
}
template <typename T>
constexpr vec3<T> operator*(T s, const vec3<T> &v) {
    return { s * v.x, s * v.y, s * v.z };
}
template <typename T>
constexpr vec3<T> operator/(T s, const vec3<T> &v) {
    return { s / v.x, s / v.y, s / v.z };
}


template <typename T>
constexpr vec4<T> operator+(T s, const vec4<T> &v) {
    return { s + v.x, s + v.y, s + v.z, s + v.w };
}
template <typename T>
constexpr vec4<T> operator-(T s, const vec4<T> &v) {
    return { s - v.x, s - v.y, s - v.z, s - v.w };
}
template <typename T>
constexpr vec4<T> operator*(T s, const vec4<T> &v) {
    return { s * v.x, s * v.y, s * v.z, s * v.w };
}
template <typename T>
constexpr vec4<T> operator/(T s, const vec4<T> &v) {
    return { s / v.x, s / v.y, s / v.z, s / v.w };
}


//=============================================================================
//-  - -- -----------=--=--==-===  Vector Math  ===-==--=--=----------- -- -  -
//=============================================================================

// Cross product
template <typename T>
constexpr vec3<T> cross(const vec3<T> &v, const vec3<T> &o) {
    return vec3<T>{
        v.y * o.z - v.z * o.y,
        v.z * o.x - v.x * o.z,
        v.x * o.y - v.y * o.x,
    };
}

// Dot product
template <typename T>
constexpr T dot(const vec2<T> &v, const vec2<T> &o) {
    return v.x * o.x + v.y * o.y;
}
template <typename T>
constexpr T dot(const vec3<T> &v, const vec3<T> &o) {
    return v.x * o.x + v.y * o.y + v.z * o.z;
}
template <typename T>
constexpr T dot(const vec4<T> &v, const vec4<T> &o) {
    return v.x * o.x + v.y * o.y + v.z * o.z + v.w * o.w;
}

// Linear interpolation
template <typename vec, typename T>
constexpr vec lerp(const vec &v, const vec &o, T a) {
    return v + (o - v) * a;
}
template <typename vec>
constexpr vec lerp(const vec &v, const vec &o, const vec &a) {
    return v + (o - v) * a;
}

// Length and distance
template <typename vec>
float length(const vec &v) {
    return std::sqrt(static_cast<float>(dot(v, v)));
}
template <typename vec>
float distance(const vec &v, const vec &o) {
    return length(v - o);
}

// Normalize
template <typename vec>
vec normalize(const vec &v) {
    return v / length(v);
}

// Surface normal manipulation
template <typename vec>
constexpr vec faceforward(const vec &N, const vec &I, const vec &Nref) {
    return dot(Nref, I) < 0 ? N : -N;
}

template <typename vec>
constexpr vec reflect(const vec &I, const vec &N) {
    return I - 2.0f * dot(N, I) * N;
}

template <typename vec>
vec refract(const vec &I, const vec &N, float eta) {
    float d = dot(N, I);
    float k = 1.0f - eta * eta * (1.0f - d * d);
    return k < 0.0f ? vec(0.0f) : I * eta - N * (eta * d + std::sqrt(k));
}


//============================================================================
//-  - -- ----------=--=--==-===  Type Aliases  ===-==--=--=---------- -- -  -
//============================================================================

using float2 = vec2<float>;
using double2 = vec2<double>;
using int2 = vec2<int>;
using uint2 = vec2<unsigned int>;
using short2 = vec2<short>;
using ushort2 = vec2<unsigned short>;
using char2 = vec2<char>;
using uchar2 = vec2<unsigned char>;
using bool2 = vec2<bool>;
using byte2 = vec2<uint8_t>;
using word2 = vec2<uint16_t>;
using dword2 = vec2<uint32_t>;
using qword2 = vec2<uint64_t>;

using float3 = vec3<float>;
using double3 = vec3<double>;
using int3 = vec3<int>;
using uint3 = vec3<unsigned int>;
using short3 = vec3<short>;
using ushort3 = vec3<unsigned short>;
using char3 = vec3<char>;
using uchar3 = vec3<unsigned char>;
using bool3 = vec3<bool>;
using byte3 = vec3<uint8_t>;
using word3 = vec3<uint16_t>;
using dword3 = vec3<uint32_t>;
using qword3 = vec3<uint64_t>;

using float4 = vec4<float>;
using double4 = vec4<double>;
using int4 = vec4<int>;
using uint4 = vec4<unsigned int>;
using short4 = vec4<short>;
using ushort4 = vec4<unsigned short>;
using char4 = vec4<char>;
using uchar4 = vec4<unsigned char>;
using bool4 = vec4<bool>;
using byte4 = vec4<uint8_t>;
using word4 = vec4<uint16_t>;
using dword4 = vec4<uint32_t>;
using qword4 = vec4<uint64_t>;