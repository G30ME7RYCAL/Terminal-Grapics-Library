#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <utility>

#include "tgl_vector.h"


// Forward declarations
template <int rows, int cols, typename T>
struct mat;

namespace detail {
    // Count components
    template <typename U>
    struct component_count {
        static constexpr std::size_t size = 1;
    };
    template <typename T>
    struct component_count<vec2<T>>   {
        static constexpr std::size_t size = 2;
    };
    template <typename T>
    struct component_count<vec3<T>>   {
        static constexpr std::size_t size = 3;
    };
    template <typename T>
    struct component_count<vec4<T>>   {
        static constexpr std::size_t size = 4;
    };
    template <int rows, int cols, typename T>
    struct component_count<mat<rows, cols, T>> {
        static constexpr std::size_t size = rows * cols;
    };

    // Flatten scalars
    template <typename T, typename U, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
    constexpr void flatten(T* out, std::size_t& idx, U scalar) {
        out[idx++] = static_cast<T>(scalar);
    }
    template <typename T, typename U>
    constexpr void flatten(T* out, std::size_t& idx, const vec2<U>& v) {
        out[idx++] = static_cast<T>(v.x);
        out[idx++] = static_cast<T>(v.y);
    }
    template <typename T, typename U>
    constexpr void flatten(T* out, std::size_t& idx, const vec3<U>& v) {
        out[idx++] = static_cast<T>(v.x);
        out[idx++] = static_cast<T>(v.y);
        out[idx++] = static_cast<T>(v.z);
    }
    template <typename T, typename U>
    constexpr void flatten(T* out, std::size_t& idx, const vec4<U>& v) {
        out[idx++] = static_cast<T>(v.x);
        out[idx++] = static_cast<T>(v.y);
        out[idx++] = static_cast<T>(v.z);
        out[idx++] = static_cast<T>(v.w);
    }

    // Flatten smaller matrices
    template <int rows, int cols, typename T, typename U>
    constexpr void flatten(T* out, std::size_t& idx, const mat<rows, cols, U>& M) {
        for (int i = 0; i < rows; i++) {
            flatten(out, idx, M.r[i]);
        }
    }
}


//============================================================================
//-  - -- ----------=--=--==-===  2 x 2 Matrix  ===-==--=--=---------- -- -  -
//============================================================================

template <int rows, int cols, typename T>
struct mat {
    static_assert(rows >= 2 && rows <= 4, "A matrix must have between 2 and 4 rows");
    static_assert(cols >= 2 && cols <= 4, "A matrix must have between 2 and 4 columns");

    int rowCount = rows;
    int colCount = cols;

    using row_type = std::conditional_t<
        cols == 2, vec2<T>,
        std::conditional_t<
            cols == 3, vec3<T>,
            vec4<T>
        >
    >;

    row_type r[rows];

    // Constructors
    constexpr mat() : r{} {}

    constexpr mat(T v) : r{} {
        for ( int i = 0; i < rows && i < cols; i++) {
            r[i][i] = v;
        }
    }

    // Variadic Constructor
    template < typename... Args,
                typename = std::enable_if_t<(sizeof...(Args) > 1) &&
                ((detail::component_count<std::decay_t<Args>>::size + ...) == rows * cols) >>
    constexpr mat(Args&&... args) : r{} {
        T flat[rows * cols]{};
        std::size_t idx = 0;
        (detail::flatten(flat, idx, std::forward<Args>(args)), ...);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                r[i][j] = flat[i * cols + j];
            }
        }
    }

    // Comparison
    constexpr bool operator==(const mat &o) const {
        for (int i = 0; i < rows; i++) {
            if (r[i] != o.r[i])
                return false;
        }
        return true;
    }
    constexpr bool operator!=(const mat &o) const {
        for (int i = 0; i < rows; i++) {
            if (r[i] != o.r[i])
                return true;
        }
        return false;
    }

    // Unary Operations
    constexpr mat operator+() const {
        return *this;
    }
    constexpr mat operator-() const {
        mat negation;

        for (int i = 0; i < rows; i++) {
            negation.r[i] = -r[i];
        }

        return negation;
    }
    constexpr mat &operator++() {
        static_assert(!std::is_same_v<T, bool>, "Cannot increment bool");
        for (int i = 0; i < rows; i++) {
            ++r[i];
        }
        return *this;
    }
    constexpr mat &operator--() {
        static_assert(!std::is_same_v<T, bool>, "Cannot decrement bool");
        for (int i = 0; i < rows; i++) {
            --r[i];
        }
        return *this;
    }
    constexpr mat operator++(int) {
        mat old = *this;
        ++(*this);
        return old;
    }
    constexpr mat operator--(int) {
        mat old = *this;
        --(*this);
        return old;
    }

    // Matrix-Matrix arithmetic
    constexpr mat operator+(const mat &o) const {
        mat sum;

        for (int i = 0; i < rows; i++) {
            sum.r[i] = r[i] + o.r[i];
        }

        return sum;
    }
    constexpr mat operator-(const mat &o) const {
        mat difference;

        for (int i = 0; i < rows; i++) {
            difference.r[i] = r[i] - o.r[i];
        }

        return difference;
    }
    constexpr mat operator*(const mat &o) const {
        mat product;

        for (int i = 0; i < rows; i++) {
            product.r[i] = r[i] * o.r[i];
        }

        return product;
    }
    constexpr mat operator/(const mat &o) const {
        mat quotient;

        for (int i = 0; i < rows; i++) {
            quotient.r[i] = r[i] / o.r[i];
        }

        return quotient;
    }

    // Matrix-Scalar arithmetic
    constexpr mat operator+(T s) const {
        mat sum;

        for (int i = 0; i < rows; i++) {
            sum.r[i] = r[i] + s;
        }

        return sum;
    }
    constexpr mat operator-(T s) const{
        mat difference;

        for (int i = 0; i < rows; i++) {
            difference.r[i] = r[i] - s;
        }

        return difference;
    }
    constexpr mat operator*(T s) const {
        mat product;

        for (int i = 0; i < rows; i++) {
            product.r[i] = r[i] * s;
        }

        return product;
    }
    constexpr mat operator/(T s) const {
        mat quotient;

        for (int i = 0; i < rows; i++) {
            quotient.r[i] = r[i] / s;
        }

        return quotient;
    }

    // Compound assignments (Matrix)
    constexpr mat &operator+=(const mat &o) {
        *this = *this + o;
        return *this;
    }
    constexpr mat &operator-=(const mat &o) {
        *this = *this - o;
        return *this;
    }
    constexpr mat &operator*=(const mat &o) {
        *this = *this * o;
        return *this;
    }
    constexpr mat &operator/=(const mat &o) {
        *this = *this / o;
        return *this;
    }

    // Compound assignments (Scalar)
    constexpr mat &operator+=(T s) {
        for (int i = 0; i < rows; i++) {
            r[i] += s;
        }
        return *this;
    }
    constexpr mat &operator-=(T s) {
        for (int i = 0; i < rows; i++) {
            r[i] -= s;
        }
        return *this;
    }
    constexpr mat &operator*=(T s) {
        for (int i = 0; i < rows; i++) {
            r[i] *= s;
        }
        return *this;
    }
    constexpr mat &operator/=(T s) {
        for (int i = 0; i < rows; i++) {
            r[i] /= s;
        }
        return *this;
    }

    // Index access
    constexpr row_type &operator[](int i) {
        if (i >= 0 && i < rows) return  r[i];
        throw std::out_of_range("2D matrix index out of range");
    }
    constexpr const row_type &operator[](int i) const {
        if (i >= 0 && i < rows) return  r[i];
        throw std::out_of_range("2D matrix index out of range");
    }
};


//============================================================================
//-  - -- -----=--=--==-=== Scalar-Matrix Arithmetic ===-==--=--=----- -- -  -
//============================================================================

template <int rows, int cols, typename T>
constexpr mat<rows, cols, T> operator+(T s, const mat<rows, cols, T> &M) {
    mat<rows, cols, T> sum;

    for (int i = 0; i < rows; i++) {
        sum.r[i] =s + M.r[i];
    }

    return sum;
}
template <int rows, int cols, typename T>
constexpr mat<rows, cols, T> operator-(T s, const mat<rows, cols, T> &M) {
    mat<rows, cols, T> difference;

    for (int i = 0; i < rows; i++) {
        difference.r[i] =s - M.r[i];
    }

    return difference;
}
template <int rows, int cols, typename T>
constexpr mat<rows, cols, T> operator*(T s, const mat<rows, cols, T> &M) {
    mat<rows, cols, T> product;

    for (int i = 0; i < rows; i++) {
        product.r[i] =s * M.r[i];
    }

    return product;
}
template <int rows, int cols, typename T>
constexpr mat<rows, cols, T> operator/(T s, const mat<rows, cols, T> &M) {
    mat<rows, cols, T> quotient;

    for (int i = 0; i < rows; i++) {
        quotient.r[i] =s / M.r[i];
    }

    return quotient;
}

//=============================================================================
//-  - -- -----------=--=--==-===  Matrix Math  ===-==--=--=----------- -- -  -
//=============================================================================

// Transpose
template <int rows, int cols, typename T>
mat<cols, rows, T> transpose(const mat<rows, cols, T> &M) {
    mat<cols, rows, T> transposed;
    for (int i = 0; i < M.rowCount; i++) {
        for (int j = 0; j < M.colCount; j++) {
            transposed[j][i] = M[i][j];
        }
    }
    return transposed;
}


// True Matrix Multiplication
template <int A_rows, int A_cols, int B_rows, int B_cols, typename T>
mat<A_rows, B_cols, T> mul(const mat<A_rows, A_cols, T> &A, const mat<B_rows, B_cols, T> &B) {
    static_assert(A_cols == B_rows, "To multiply 2 matrices, their inner dimensions must match");
    mat<A_rows, B_cols, T> product;
    for (int i = 0; i < A.rowCount; i++) {
        for (int j = 0; j < B.colCount; j++) {
            product[i][j] = dot(A[i], transpose(B)[j]);
        }
    }
    return product;
}

template <int rows, int cols, typename T, typename vec>
vec mul(const mat<rows, cols, T> &M, const vec &v) {
    vec product;
    for (int i = 0; i < M.rowCount; i++) {
        product[i] = dot(M[i], v);
    }
    return product;
}

template <typename vec, int rows, int cols, typename T>
vec mul(const vec &v, const mat<rows, cols, T> &M) {
    vec product;
    for (int i = 0; i < M.rowCount; i++) {
        product[i] = dot(v, transpose(M)[i]);
    }
    return product;
}

// Determinant
template <typename T>
float det(const mat<2, 2, T> &M) {
    T a = M[0][0], b = M[0][1];
    T c = M[1][0], d = M[1][1];

    return a*d - b*c;
}

template <typename T>
float det(const mat<3, 3, T> &M) {
    T a = M[0][0], b = M[0][1], c = M[0][2];
    T d = M[1][0], e = M[1][1], f = M[1][2];
    T g = M[2][0], h = M[2][1], i = M[2][2];
    
    return a * det(mat<2, 2, T>(e, f,
                                h, i)) -

           b * det(mat<2, 2, T>(d, f,
                                g, i)) +

           c * det(mat<2, 2, T>(d, e,
                                g, h));
}

template <typename T>
float det(const mat<4, 4, T> &M) {
    T a = M[0][0], b = M[0][1], c = M[0][2], d = M[0][3];
    T e = M[1][0], f = M[1][1], g = M[1][2], h = M[1][3];
    T i = M[2][0], j = M[2][1], k = M[2][2], l = M[2][3];
    T m = M[3][0], n = M[3][1], o = M[3][2], p = M[3][3];
    
    return a * det(mat<3, 3, T>(f, g, h,
                                j, k, l,
                                n, o, p)) -

           b * det(mat<3, 3, T>(e, g, h,
                                i, k, l,
                                m, o, p)) +

           c * det(mat<3, 3, T>(e, f, h,
                                i, j, l,
                                m, n, p)) -

           d * det(mat<3, 3, T>(e, f, g,
                                i, j, k,
                                m, n, o));
}

//============================================================================
//-  - -- ----------=--=--==-===  Type Aliases  ===-==--=--=---------- -- -  -
//============================================================================

using float2x2 = mat<2, 2, float>;
using double2x2 = mat<2, 2, double>;
using int2x2 = mat<2, 2, int>;
using uint2x2 = mat<2, 2, unsigned int>;
using short2x2 = mat<2, 2, short>;
using ushort2x2 = mat<2, 2, unsigned short>;
using char2x2 = mat<2, 2, char>;
using uchar2x2 = mat<2, 2, unsigned char>;
using bool2x2 = mat<2, 2, bool>;
using byte2x2 = mat<2, 2, uint8_t>;
using word2x2 = mat<2, 2, uint16_t>;
using dword2x2 = mat<2, 2, uint32_t>;
using qword2x2 = mat<2, 2, uint64_t>;

using float3x2 = mat<3, 2, float>;
using double3x2 = mat<3, 2, double>;
using int3x2 = mat<3, 2, int>;
using uint3x2 = mat<3, 2, unsigned int>;
using dword3x2 = mat<3, 2, unsigned int>;
using short3x2 = mat<3, 2, short>;
using ushort3x2 = mat<3, 2, unsigned short>;
using char3x2 = mat<3, 2, char>;
using uchar3x2 = mat<3, 2, unsigned char>;
using bool3x2 = mat<3, 2, bool>;
using byte3x2 = mat<3, 2, uint8_t>;
using word3x2 = mat<3, 2, uint16_t>;
using dword3x2 = mat<3, 2, uint32_t>;
using qword3x2 = mat<3, 2, uint64_t>;

using float4x2 = mat<4, 2, float>;
using double4x2 = mat<4,2 , double>;
using int4x2 = mat<4, 2, int>;
using uint4x2 = mat<4, 2, unsigned int>;
using dword4x2 = mat<4,2 , unsigned int>;
using short4x2 = mat<4, 2, short>;
using ushort4x2 = mat<4, 2, unsigned short>;
using char4x2 = mat<4, 2, char>;
using uchar4x2 = mat<4, 2, unsigned char>;
using bool4x2 = mat<4, 2, bool>;
using byte4x2 = mat<4, 2, uint8_t>;
using word4x2 = mat<4, 2, uint16_t>;
using dword4x2 = mat<4, 2, uint32_t>;
using qword4x2 = mat<4, 2, uint64_t>;



using float2x3 = mat<2, 3, float>;
using double2x3 = mat<2, 3, double>;
using int2x3 = mat<2, 3, int>;
using uint2x3 = mat<2, 3, unsigned int>;
using dword2x3 = mat<2, 3, unsigned int>;
using short2x3 = mat<2, 3, short>;
using ushort2x3 = mat<2, 3, unsigned short>;
using char2x3 = mat<2, 3, char>;
using uchar2x3 = mat<2, 3, unsigned char>;
using bool2x3 = mat<2, 3, bool>;
using byte2x3 = mat<2, 3, uint8_t>;
using word2x3 = mat<2, 3, uint16_t>;
using dword2x3 = mat<2, 3, uint32_t>;
using qword2x3 = mat<2, 3, uint64_t>;

using float3x3 = mat<3, 3, float>;
using double3x3 = mat<3, 3, double>;
using int3x3 = mat<3, 3, int>;
using uint3x3 = mat<3, 3, unsigned int>;
using dword3x3 = mat<3, 3, unsigned int>;
using short3x3 = mat<3, 3, short>;
using ushort3x3 = mat<3, 3, unsigned short>;
using char3x3 = mat<3, 3, char>;
using uchar3x3 = mat<3, 3, unsigned char>;
using bool3x3 = mat<3, 3, bool>;
using byte3x3 = mat<3, 3, uint8_t>;
using word3x3 = mat<3, 3, uint16_t>;
using dword3x3 = mat<3, 3, uint32_t>;
using qword3x3 = mat<3, 3, uint64_t>;

using float4x3 = mat<4, 3, float>;
using double4x3 = mat<4, 3, double>;
using int4x3 = mat<4, 3, int>;
using uint4x3 = mat<4, 3, unsigned int>;
using dword4x3 = mat<4, 3, unsigned int>;
using short4x3 = mat<4, 3, short>;
using ushort4x3 = mat<4, 3, unsigned short>;
using char4x3 = mat<4, 3, char>;
using uchar4x3 = mat<4, 3, unsigned char>;
using bool4x3 = mat<4, 3, bool>;
using byte4x3 = mat<4, 3, uint8_t>;
using word4x3 = mat<4, 3, uint16_t>;
using dword4x3 = mat<4, 3, uint32_t>;
using qword4x3 = mat<4, 3, uint64_t>;


using float2x4 = mat<2, 4, float>;
using double2x4 = mat<2, 4, double>;
using int2x4 = mat<2, 4, int>;
using uint2x4 = mat<2, 4, unsigned int>;
using dword2x4 = mat<2, 4, unsigned int>;
using short2x4 = mat<2, 4, short>;
using ushort2x4 = mat<2, 4, unsigned short>;
using char2x4 = mat<2, 4, char>;
using uchar2x4 = mat<2, 4, unsigned char>;
using bool2x4 = mat<2, 4, bool>;
using byte2x4 = mat<2, 4, uint8_t>;
using word2x4 = mat<2, 4, uint16_t>;
using dword2x4 = mat<2, 4, uint32_t>;
using qword2x4 = mat<2, 4, uint64_t>;

using float3x4 = mat<3, 4, float>;
using double3x4 = mat<3, 4, double>;
using int3x4 = mat<3, 4, int>;
using uint3x4 = mat<3, 4, unsigned int>;
using dword3x4 = mat<3, 4, unsigned int>;
using short3x4 = mat<3, 4, short>;
using ushort3x4 = mat<3, 4, unsigned short>;
using char3x4 = mat<3, 4, char>;
using uchar3x4 = mat<3, 4, unsigned char>;
using bool3x4 = mat<3, 4, bool>;
using uchar3x4 = mat<3, 4, unsigned char>;
using bool3x4 = mat<3, 4, bool>;
using byte3x4 = mat<3, 4, uint8_t>;
using word3x4 = mat<3, 4, uint16_t>;
using dword3x4 = mat<3, 4, uint32_t>;
using qword3x4 = mat<3, 4, uint64_t>;

using float4x4 = mat<4, 4, float>;
using double4x4 = mat<4, 4, double>;
using int4x4 = mat<4, 4, int>;
using uint4x4 = mat<4, 4, unsigned int>;
using dword4x4 = mat<4, 4, unsigned int>;
using short4x4 = mat<4, 4, short>;
using ushort4x4 = mat<4, 4, unsigned short>;
using char4x4 = mat<4, 4, char>;
using uchar4x4 = mat<4, 4, unsigned char>;
using bool4x4 = mat<4, 4, bool>;
using byte4x4 = mat<4, 4, uint8_t>;
using word4x4 = mat<4, 4, uint16_t>;
using dword4x4 = mat<4, 4, uint32_t>;
using qword4x4 = mat<4, 4, uint64_t>;