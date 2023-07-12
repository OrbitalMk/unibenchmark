/* benchmark.c
 *
 * Copyright 2023 OrbitalMk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "benchmark.h"

#include <glib/gi18n.h>

#define SIZE 50000
#define SEED 1234
#define MOD 100000

static guint64
bubble_sort(int *array, size_t len, GtkProgressBar *progress) {
    guint64 ciclos = 0;
    size_t l = len - 1;

    for (size_t i = 0; i < l; i++) {
        if ((i + 1) % 5000 == 0) {
            gtk_progress_bar_set_fraction(progress, ((double) i) / SIZE);
        }

        for (size_t j = 0; j < l; j++) {
            ciclos++;

            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    gtk_progress_bar_set_fraction(progress, 1.0f);

    return ciclos;
}

void
benchmark(benchmark_info *info, GtkProgressBar *progress) {
    int array[SIZE];
    clock_t start, end;

    start = clock();

    srand(SEED);

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % MOD;
    }

    info->ciclos = bubble_sort(array, SIZE, progress);

    end = clock();

    info->seg = (double) (end - start) / CLOCKS_PER_SEC;
}
