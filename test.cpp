// creact.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Reactive.hpp"


Reactive<int> Test1()
{
    Reactive<int> x(5), y(6), z(99);

    std::cout << "z=" << z << std::endl;

    z = x + y;

    std::cout << "after assigning z=x+y , z=" << z << std::endl;

    x = 1; // change x

    // z is automatically updated
    std::cout << "after changing x:  z=" << z << std::endl;

    y = 1; // change y

    // z is automatically updated
    std::cout << "after changing y:  z=" << z << std::endl;

    Reactive<int> fn = z;


    std::cout << "fn = " << fn << std::endl;

    x = 3 + y; // change x

    // z and fn  are automatically updated
    std::cout << "after changing x:  z=" << z << ", fn=" << fn << std::endl;


    return fn;
}

template<typename T, size_t N>
void PrintSqrMatrix2D(T(&mat)[N][N])
{
    std::cout << "Result matrix is \n";
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++)
            std::cout << mat[i][j] << " ";
        std::cout << "\n";
    }

}

void Test2()
{
    constexpr size_t N = 4;
    Reactive<int> res[N][N]; // To store result
    Reactive<int> mat1[N][N] = {
        { 1, 1, 1, 1 },
        { 2, 2, 2, 2 },
        { 3, 3, 3, 3 },
        { 4, 4, 4, 4 } };

    Reactive<int> mat2[N][N] = { 
        { 1, 1, 1, 1 },
        { 2, 2, 2, 2 },
        { 3, 3, 3, 3 },
        { 4, 4, 4, 4 } };
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            res[i][j] =
                mat1[i][0] * mat2[0][j] +
                mat1[i][1] * mat2[1][j] +
                mat1[i][2] * mat2[2][j] +
                mat1[i][3] * mat2[3][j];
        }
    }

    PrintSqrMatrix2D(res);

    mat1[0][0] = 3; // this change will propagate to the result matrix 'res' automatically

    PrintSqrMatrix2D(res);



}

int main()
{
    auto fn = Test1();
    std::cout << fn << std::endl;

    Test2();
}

