#include <cassert>
#include <iostream>
#include "PolyLines.h"

using namespace std;

void testPolyLineConstructor() {
    std::random_device rd;
    std::mt19937 gen(rd());


    // Тест 1: double
    PolyLine<double> pl1(static_cast<size_t>(5), 1.0, 10.0);
    assert(pl1.GetSize() == 5);
    for (size_t i = 0; i < pl1.GetSize(); ++i) {
        assert(pl1[i].x >= 1.0 && pl1[i].x <= 10.0);
        assert(pl1[i].y >= 1.0 && pl1[i].y <= 10.0);
    }

    // Тест 2: float
    PolyLine<float> pl2(static_cast<size_t>(3), 2.5f, 7.5f);
    assert(pl2.GetSize() == 3);
    for (size_t i = 0; i < pl2.GetSize(); ++i) {
        assert(pl2[i].x >= 2.5f && pl2[i].x <= 7.5f);
        assert(pl2[i].y >= 2.5f && pl2[i].y <= 7.5f);
    }

    // Тест 3: complex<double>
    PolyLine<std::complex<double>> pl3(2, { 1.0, 2.0 }, { 5.0, 6.0 });
    assert(pl3.GetSize() == 2);
    for (size_t i = 0; i < pl3.GetSize(); ++i) {
        assert(pl3[i].x.real() >= 1.0 && pl3[i].x.real() <= 5.0);
        assert(pl3[i].x.imag() >= 2.0 && pl3[i].x.imag() <= 6.0);
        assert(pl3[i].y.real() >= 1.0 && pl3[i].y.real() <= 5.0);
        assert(pl3[i].y.imag() >= 2.0 && pl3[i].y.imag() <= 6.0);
    }

    // Тест 4: complex<float>
    PolyLine<std::complex<float>> pl4(1, { 1.0f, 2.0f }, { 5.0f, 6.0f });
    assert(pl4.GetSize() == 1);
    assert(pl4[0].x.real() >= 1.0f && pl4[0].x.real() <= 5.0f);
    assert(pl4[0].x.imag() >= 2.0f && pl4[0].x.imag() <= 6.0f);
    assert(pl4[0].y.real() >= 1.0f && pl4[0].y.real() <= 5.0f);
    assert(pl4[0].y.imag() >= 2.0f && pl4[0].y.imag() <= 6.0f);

    // Тест 5: int (положительные числа)
    PolyLine<int> pl5(static_cast<size_t>(4), 1, 10);
    assert(pl5.GetSize() == 4);
    for (size_t i = 0; i < pl5.GetSize(); ++i) {
        assert(pl5[i].x >= 1 && pl5[i].x <= 10);
        assert(pl5[i].y >= 1 && pl5[i].y <= 10);
    }

    // Тест 6: int (отрицательные числа)
    PolyLine<int> pl6(static_cast<size_t>(2), -5, -1);
    assert(pl6.GetSize() == 2);
    for (size_t i = 0; i < pl6.GetSize(); ++i) {
        assert(pl6[i].x >= -5 && pl6[i].x <= -1);
        assert(pl6[i].y >= -5 && pl6[i].y <= -1);
    }

    // Тест 7: unsigned int
    PolyLine<unsigned int> pl7(static_cast<size_t>(3), 1u, 10u);
    assert(pl7.GetSize() == 3);
    for (size_t i = 0; i < pl7.GetSize(); ++i) {
        assert(pl7[i].x >= 1u && pl7[i].x <= 10u);
        assert(pl7[i].y >= 1u && pl7[i].y <= 10u);
    }
}

void testPolyLine() {
    // Тест 1: Конструктор по умолчанию
    PolyLine<double> pl1;
    assert(pl1.GetSize() == 0);

    // Тест 2: Конструктор с одной точкой
    Point<double> p1(1.0, 2.0);
    PolyLine<double> pl2(p1);
    assert(pl2.GetSize() == 1);
    assert(pl2[0].x == 1.0);
    assert(pl2[0].y == 2.0);

    // Тест 3: Конструктор с заданным количеством точек
    PolyLine<double> pl3(3);
    assert(pl3.GetSize() == 3);

    // Тест 4: Конструктор с заданным количеством точек и диапазоном
    PolyLine<double> pl4(static_cast<size_t>(2), 1.0, 5.0);
    assert(pl4.GetSize() == 2); // Проверка количества точек

    // Тест 5: Конструктор для прямоугольника
    PolyLine<double> pl5(2.0, 4.0, 3.0); // прямоугольник 2x3
    assert(pl5.GetSize() == 5); // Проверка количества точек

    // Тест 6:  GetLength()
    PolyLine<double> pl6;
    pl6.PushBack({ 1.0, 1.0 });
    pl6.PushBack({ 4.0, 5.0 });
    pl6.PushBack({ 7.0, 1.0 });
    long double len = pl6.GetLength();
    assert(abs(len - 10.0) < 1e-6); // проверка с учетом погрешности вычислений

    // Тест 7: оператор + (PolyLine + PolyLine)
    PolyLine<double> pl7;
    pl7.PushBack({ 1,1 });
    PolyLine<double> pl8 = pl6 + pl7;
    assert(pl8.GetSize() == 4);

    // Тест 8: оператор + (PolyLine + Point)
    PolyLine<double> pl9 = pl6 + Point<double>(10.0, 10.0);
    assert(pl9.GetSize() == 4);


    // Тест 9: оператор += (PolyLine += PolyLine)
    PolyLine<double> pl10;
    pl10.PushBack({ 2,2 });
    pl6 += pl10;
    assert(pl6.GetSize() == 4);

    // Тест 10: оператор += (PolyLine += Point)
    pl6 += Point<double>(3, 3);
    assert(pl6.GetSize() == 5);


    // Тесты с комплексными числами
    PolyLine<std::complex<double>> pl_complex;
    pl_complex.PushBack({ {1.0, 2.0}, {3.0, 4.0} });
    pl_complex.PushBack({ {5.0, 6.0}, {7.0, 8.0} });
    long double complex_len = pl_complex.GetLength();
    // Точное значение сложно вычислить аналитически, поэтому проверяем на разумность
    assert(complex_len > 0);
}

int main() {
    testPolyLineConstructor();
    testPolyLine();
    long double a, b, h;
    cout << "Enter enter point value\n";
    cout << "a=";
    cin >> a;
    cout << "b=";
    cin >> b;
    cout << "h=";
    cin >> h;
    //cin >> a >> b >> h;
    PolyLine<long double> pl(a, b, h);
    cout << "Perimeter of the trapezoid=";
    cout << pl.GetLength()<<"\n";
    cout << "Distance between points=";
    cout << pl.CalculateLength(pl[1], pl[2]);

    return 0;
}