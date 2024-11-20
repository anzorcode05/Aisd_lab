#pragma once

#include <iostream>
#include <cmath> 
#include <complex>
#include <random>
#include <type_traits>
#include "MyList.h"

static const long double epsilon = 0.000001;

template <typename T>
class Point {
public:
    T x;
    T y;

    Point() : x(0), y(0) {}
    Point(T x, T y) : x(x), y(y) {}

    bool operator==(const Point& arg) const {
        if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
            return std::abs(x - arg.x) < epsilon && std::abs(y - arg.y) < epsilon;
        }
        if constexpr (std::is_same_v<T, std::complex<double>>) {
            return std::abs(x.real() - arg.x.real()) < epsilon &&
                std::abs(x.imag() - arg.x.imag()) < epsilon &&
                std::abs(y.real() - arg.y.real()) < epsilon &&
                std::abs(y.imag() - arg.y.imag()) < epsilon;
        }
        if constexpr (std::is_integral_v<T>) {
            return x == arg.x && y == arg.y; // Точное сравнение для целых чисел
        }
    }

    bool operator!=(const Point<T>& arg) const {
        return !(*this == arg);
    }
};


template <typename T>
bool compare_less(const T& a, const T& b) {
    if constexpr (std::is_floating_point_v<T>) {
        return a < b;
    }
    if constexpr (std::is_integral_v<T>) {
        return a < b;
    }
    if constexpr (std::is_same_v<T, std::complex<double>> || std::is_same_v<T, std::complex<float>>) {
        return std::abs(a) < std::abs(b);
    }
}

template <typename T>
class PolyLine {
private:
    MyList<Point<T>> m_points;
public:
    PolyLine() {}

    PolyLine(const Point<T>& point) { m_points.PushBack(point); }

    PolyLine(size_t points_count) {
        for (size_t i = 0; i < points_count; i++) {
            m_points.PushBack({});
        }
    }

    PolyLine(size_t points_count, T from, T to) {
        if (compare_less(to, from)) std::swap(from, to);

        std::random_device rd;
        std::mt19937 gen(rd());

        if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<double> distrib(static_cast<double>(from), static_cast<double>(to));
            for (size_t i = 0; i < points_count; ++i) {
                T x = static_cast<T>(distrib(gen));
                T y = static_cast<T>(distrib(gen));
                m_points.PushBack({ x, y });
            }
            return;
        }

        if constexpr (std::is_same_v<T, std::complex<double>> || std::is_same_v<T, std::complex<float>>) {
            using RealType = typename std::conditional<std::is_same_v<T, std::complex<double>>, double, float>::type;
            std::uniform_real_distribution<double> real_distrib(from.real(), to.real());
            std::uniform_real_distribution<double> imag_distrib(from.imag(), to.imag());
            for (size_t i = 0; i < points_count; ++i) {
                T x = { static_cast<RealType>(real_distrib(gen)), static_cast<RealType>(imag_distrib(gen)) };
                T y = { static_cast<RealType>(real_distrib(gen)), static_cast<RealType>(imag_distrib(gen)) };
                m_points.PushBack({ x, y });
            }
            return;
        }

        if constexpr (std::is_integral_v<T>) {
            using UnderlyingType = std::make_unsigned_t<T>;
            std::uniform_int_distribution<UnderlyingType> distrib(0, static_cast<UnderlyingType>(to - from));
            for (size_t i = 0; i < points_count; ++i) {
                T x = static_cast<T>(distrib(gen)) + from;
                T y = static_cast<T>(distrib(gen)) + from;
                m_points.PushBack({ x, y });
            }
        }
    }

    PolyLine(const PolyLine<T>& rhs) {
        m_points = rhs.m_points;
    }

    PolyLine(const T& a, const T& b, const T& h) {
        Point<T> point0(0, 0);
        m_points.PushBack(point0);
        Point<T> point1(a, 0);
        m_points.PushBack(point1);
        Point<T> point2(b, h);
        m_points.PushBack(point2);
        Point<T> point3(0, h);
        m_points.PushBack(point3);
        m_points.PushBack(point0);
    }

    PolyLine<T>& operator=(const PolyLine<T>& rhs) {
        if (this != &rhs) {
            m_points = rhs.m_points;
        }
        return *this;
    }

    long double GetLength() const {
        if (m_points.Size() < 2) return 0.;
        long double length = 0.;
        for (size_t i = 1; i < m_points.Size(); ++i) {
            length += CalculateLength(m_points[i - 1], m_points[i]);
        }
        return length;
    }

    size_t GetSize() const { return m_points.Size(); }

    void PushBack(const Point<T>& point) { m_points.PushBack(point); }

    void PushFront(const Point<T>& point) { m_points.PushFront(point); }

    static long double CalculateLength(const Point<T>& point1, const Point<T>& point2) {
        return std::sqrt(abs(point1.x - point2.x) * abs(point1.x - point2.x) +
            abs(point1.y - point2.y) * abs(point1.y - point2.y));
    }

    Point<T>& operator[] (size_t idx) { return m_points[idx]; }

    const Point<T>& operator[] (size_t idx) const { return m_points[idx]; }

    bool operator==(const PolyLine& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }
        for (int i = 0; i < GetSize(); ++i) {
            if (this->operator[](i) != other.operator[](i)) return false;
        }
        return true;
    }

    bool operator!=(const PolyLine& other) const { return !(*this == other); }

    PolyLine operator+(const PolyLine<T>& arg) {
        PolyLine<T> result = *this;
        for (int i = 0; i < arg.GetSize(); ++i) {
            result.PushBack(arg[i]);
        }
        return result;
    }

    PolyLine operator+(const Point<T>& point) {
        PolyLine<T> result = *this;
        result.PushBack(point);
        return result;
    }

    PolyLine& operator+=(const PolyLine<T>& rhs) {
        for (int i = 0; i < rhs.GetSize(); ++i) {
            PushBack(rhs[i]);
        }
        return *this;
    }

    PolyLine& operator+=(const Point<T>& point) {
        PushBack(point);
        return *this;
    }
};

template <typename T>
PolyLine<T> operator+(const Point<T>& point, PolyLine<T> lhs) {
    lhs.PushFront(point);
    return lhs;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const Point<T>& point)
{
    return os << '{' << point.x << ", " << point.y << '}';
}

template <typename T>
std::ostream& operator << (std::ostream& os, const PolyLine<T>& pl)
{
    for (size_t i = 0; i < pl.GetSize(); ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << pl[i];
    }
    return os;
}