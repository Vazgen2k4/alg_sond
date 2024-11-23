#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using namespace std;

typedef struct matrix_data {
  int rows;
  int cols;
  int sonds_count;
  vector<vector<int>> matrix;
  matrix_data() : rows(0), cols(0), sonds_count(0), matrix({}) {}

} matrix_data;

typedef tuple<int, int, int> result;

// vector<vector<int>> plain_matrix;
// {vakue, {{x1, y1}, {x2, y2}...}}
map<int, vector<pair<int, int>>> value_coords;

void read_input(matrix_data& data, vector<int>& values_sonds,
                istream& in = cin);

result find_best_placement(int current_sond, vector<int>& sond_ranges,
                               vector<pair<int, int>>& placed_sonds,
                               vector<vector<bool>>& visited,
                               const matrix_data& data, int monitored_cells,
                               int monitored_height_sum, int placed_height_sum);

#ifndef TEST

int main() {
  matrix_data data;
  vector<int> sond_ranges;

  // Read input
  read_input(data, sond_ranges);

  vector<vector<bool>> visited(data.rows, vector<bool>(data.cols, false));
  vector<pair<int, int>> placed_sonds;

  // Find the best placement using the recursive function
  auto result =
      find_best_placement(0, sond_ranges, placed_sonds, visited, data, 0, 0, 0);

  // Output the result
  cout << get<0>(result) << " " << get<1>(result) << " " << get<2>(result)
       << endl;
  return 0;
}

#endif

void read_input(matrix_data& data, vector<int>& values_sonds, istream& in) {
  in >> data.rows >> data.cols;
  data.matrix.resize(data.rows, vector<int>(data.cols, 0));
  // plain_matrix.resize(data.rows, vector<int>(data.cols, 0));

  for (int i = 0; i < data.rows; i++) {
    for (int j = 0; j < data.cols; j++) {
      in >> data.matrix[i][j];
      int val = data.matrix[i][j];
      value_coords[val].push_back({i, j});
    }
  }

  for (auto& [key, vec] : value_coords) {
    cout << key << ": ";
    for (auto& i : vec) {
      cout << "{" << i.first << "," << i.second << "} ";
    }
    cout << endl;
  }
  in >> data.sonds_count;
  values_sonds.resize(data.sonds_count, 0);
  for (int i = 0; i < data.sonds_count; i++) {
    in >> values_sonds[i];
  }
}

result find_best_placement(int current_sond, vector<int>& sond_ranges,
                               vector<pair<int, int>>& placed_sonds,
                               vector<vector<bool>>& visited,
                               const matrix_data& data, int monitored_cells,
                               int monitored_height_sum, int placed_height_sum) {
    // Базовый случай: все зонды размещены
    if (current_sond == sond_ranges.size()) {
        return {monitored_cells, monitored_height_sum, placed_height_sum};
    }

    result best_result = {0, 0, INT_MAX}; // Изначально наилучший результат пустой

    // Перебор всех клеток для размещения текущей сонд
    for (int i = 0; i < data.rows; ++i) {
        for (int j = 0; j < data.cols; ++j) {
            // Пропустить, если клетка уже занята
            if (visited[i][j]) continue;

            // Пометить клетку как занято для текущей сонд
            visited[i][j] = true;
            placed_sonds.push_back({i, j});

            // Текущие параметры размещения сонд: количество охваченных клеток и высота
            int new_monitored_cells = monitored_cells;
            int new_monitored_height_sum = monitored_height_sum;
            vector<pair<int, int>> updated_cells; // Список новых клеток, которые покрываются

            // Радиус покрытия для текущей сонд
            int range = sond_ranges[current_sond];
            int radius_cells = 0;
            int radius_height_sum = 0;

            // Расширение по вертикали и горизонтали в пределах радиуса
            for (int di = -range; di <= range; ++di) {
                for (int dj = -range; dj <= range; ++dj) {
                    // Если клетка в пределах диапазона (по вертикали или горизонтали)
                    if (di == 0 || dj == 0) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < data.rows && nj >= 0 && nj < data.cols) {
                            // Если клетка еще не посещена, то она добавляется в покрытие
                            if (!visited[ni][nj]) {
                                visited[ni][nj] = true;
                                updated_cells.push_back({ni, nj});
                                radius_cells++;
                                radius_height_sum += data.matrix[ni][nj];
                            }
                        }
                    }
                }
            }

            // Рекурсивный вызов для следующей сонд
            auto result = find_best_placement(
                current_sond + 1, sond_ranges, placed_sonds, visited, data,
                new_monitored_cells + radius_cells, new_monitored_height_sum + radius_height_sum,
                placed_height_sum + data.matrix[i][j]);

            // Сравнение текущего результата с лучшим
            if (get<0>(result) > get<0>(best_result) ||
                (get<0>(result) == get<0>(best_result) && get<1>(result) > get<1>(best_result)) ||
                (get<0>(result) == get<0>(best_result) && get<1>(result) == get<1>(best_result) && get<2>(result) < get<2>(best_result))) {
                best_result = result;
            }

            // Откат изменений для backtracking
            for (auto& cell : updated_cells) {
                visited[cell.first][cell.second] = false;
            }
            visited[i][j] = false;
            placed_sonds.pop_back();
        }
    }

    return best_result;
}
