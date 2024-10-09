#include <iostream>
#include <omp.h>
#include <locale.h>


int main() {
    const int num_workers = 30;
    const int total_items = 20000;

    int items_ostatok = total_items;
    int items_per_worker[num_workers];
	std::setlocale(LC_ALL, "ru_RU.UTF-8");

    #pragma omp parallel num_threads(num_workers)
    {
        int worker_id = omp_get_thread_num();
        
        while (items_ostatok > 0) {
            // Кол-во предметов для текущего работника
            #pragma omp critical
			{
            items_per_worker[worker_id] = (items_ostatok / (num_workers * 2)) + 1;
			}
            // Уменьшение предметов на взятое работником в данном потоке (посчитал через атомарную операцию):
            #pragma omp atomic
            items_ostatok -= items_per_worker[worker_id];

            #pragma omp critical
            {
                // Вывод так, чтобы номера работников были от 1 до 30:
                std::cout << "Рабочий под номером " << worker_id+1 << " берёт " << items_per_worker[worker_id] << " предметов." << std::endl;
            }
			// Задержка для визуализации параллельности
			#pragma omp barrier
        }
    }

    return 0;
}
