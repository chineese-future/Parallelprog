#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
// Мин. значение элемента матрицы
constexpr size_t Min = 0;
// Макс. значение элемента матрицы
constexpr size_t Max = 100;
// Кол-во потоков каждого случая
constexpr size_t THREADS_COUNT[4] = { 1, 2, 4, 10 };

/*
    Заполнение матрицы случайными числами
    matrix - исходная матрица
    size_t A - высота
    size_t B - ширина
*/
void filling(std::vector<std::vector<int>>& matrix, size_t A, size_t B)
{
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
    // высота
    size_t A = rand() % (3000 - 2000 + 1) + 2000;

    // ширина
    size_t B = rand() % (3000 - 2000 + 1) + 2000;
    std::vector<std::vector<int>> matrix(A, std::vector<int>(B, 1));
    std::ofstream out;
    out.open("result.txt");

    filling(matrix, A, B);

    // Основной цикл выполнения (рассматирваем 4 случая)
    for (size_t k = 0; k < 4; k++)
    {
        const size_t threads_count = THREADS_COUNT[k];
        float* results = new float[threads_count];
        for (size_t i = 0; i < threads_count; i++)
            results[i] = 0;

        // Задание числа потоков
        std::thread* threads = new std::thread[threads_count];

        // Задаем количество элементов для каждой части, одна часть - один поток выполнения
        size_t amount_of_work = A / threads_count;

        // Создание потоков
        for (size_t i = 0; i < threads_count; i++)
        {
            threads[i] = std::thread(summation, std::ref(matrix), amount_of_work * i, (i == threads_count + 1 ? A : amount_of_work * (i + 1)), B, std::ref(results[i]));
        }

        // Запускаем все используемые потоки и замеряем время их выпополнения
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < threads_count; ++i)
        {
            threads[i].join();
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> milli_diff = end - start;
        
        
        float result = 0;
        for (size_t i = 0; i < threads_count; ++i)
        {
            result += results[i];
        }
        
        out << "threads_count: " << threads_count << std::endl;
        out << "time: " << milli_diff.count() << std::endl;
    }
    out.close();

    return 0;
}
