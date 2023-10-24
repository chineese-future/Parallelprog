#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <fstream>

constexpr size_t Min = 0;
constexpr size_t Max = 100;
constexpr size_t THREADS_COUNT[4] = { 1, 2, 4, 10 };

void filling(std::vector<std::vector<int>>& matrix, size_t A, size_t B)
{
    #pragma omp parallel for
    for (size_t i = 0; i < A; ++i)
    {
        for (size_t j = 0; j < B; ++j)
        {
            matrix[i][j] = rand() % (Max - Min + 1) + Max;
        }
    }
}

void summation(std::vector<std::vector<int>>& matrix, size_t a, size_t b, size_t B, float& result)
{
    for (size_t i = a + 1; i < b; i++)
    {
        for (size_t j = 0; j < B; j++)
        {
            result += matrix[i][j];
        }
    }
}

int main()
{
    srand(time(0));
    size_t A = rand() % (3000 - 2000 + 1) + 2000;
    size_t B = rand() % (3000 - 2000 + 1) + 2000;
    std::vector<std::vector<int>> matrix(A, std::vector<int>(B, 1));
    
    filling(matrix, A, B);

    std::ofstream out;
    out.open("result.txt");

    for (size_t k = 0; k < 4; k++)
    {
        const size_t threads_count = THREADS_COUNT[k];
        float result = 0;

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        #pragma omp parallel for reduction(+:result) num_threads(threads_count)
        for (size_t i = 0; i < A; ++i)
        {
			for (size_t j = 0; j < B; j++){
				result += matrix[i][j];
			}
            //summation(matrix, i, i + 1, B, result);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> milli_diff = end - start;
        
        out << "threads_count: " << threads_count << std::endl;
        out << "time: " << milli_diff.count() << std::endl;
    }

    out.close();

    return 0;
}
