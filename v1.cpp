#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

enum cell_state { EMPTY = 0, SOND = 1, COVERED = 2 };

typedef struct matrix_data {
  int rows;
  int cols;
  int sonds_count;
  vector<vector<int>> matrix;
  vector<int> radiuses_sonds;
  matrix_data()
      : rows(0), cols(0), sonds_count(0), matrix({}), radiuses_sonds({}) {}

} matrix_data;

typedef struct sond {
  int x;
  int y;
  int radius;

  sond(int x, int y, int radius) : x(x), y(y), radius(radius) {}
  sond() : x(0), y(0), radius(0) {}
} sond;

typedef tuple<int, int, int> result;
typedef map<int, pair<int, pair<int, int>>> otladka;
// матрица с нулями
// матрица для размерения
vector<vector<int>> plain_matrix;

// значения кординат для каждой цифры от 1 до 9
map<int, vector<pair<int, int>>> value_coords = {
    {9, {}}, {8, {}}, {7, {}}, {6, {}}, {5, {}},
    {4, {}}, {3, {}}, {2, {}}, {1, {}},
};

void read_input(matrix_data& data, istream& in = cin);

result back_tracking(matrix_data& data, otladka& sonds_cords, int depth = 0);

int get_step(int dy);

void print_matrix(const matrix_data& data, ostream& out);
void print_matrix(const vector<vector<int>>& data, ostream& out);

bool in_size(int coord, int max_size);
bool in_evklid(int dx, int dy, int radius);

#ifndef TEST

int main(int argc, char const* argv[]) {
  matrix_data data;

  otladka precounted_sonds_sums;
  read_input(data);

  result res = back_tracking(data, precounted_sonds_sums);

  cout << get<0>(res) << " " << get<1>(res) << " " << get<2>(res) << endl;

  return 0;
}
#endif

void precount_sond_min_max_sum(int sond_val) {
  int res = 1;  // sond point
}

void read_input(matrix_data& data, istream& in) {
  in >> data.rows >> data.cols;
  data.matrix.resize(data.rows, vector<int>(data.cols, 0));
  plain_matrix.resize(data.rows, vector<int>(data.cols, 0));

  for (int i = 0; i < data.rows; i++) {
    for (int j = 0; j < data.cols; j++) {
      in >> data.matrix[i][j];
      int val = data.matrix[i][j];
      value_coords[val].push_back({i, j});
    }
  }

  in >> data.sonds_count;
  data.radiuses_sonds.resize(data.sonds_count, 0);

  for (int i = 0; i < data.sonds_count; i++) {
    in >> data.radiuses_sonds[i];
  }

  reverse(data.radiuses_sonds.begin(), data.radiuses_sonds.end());

  for (auto& i : data.radiuses_sonds) {
    cout << "sond_value(" << i << ")\n";
  }
}

void print_matrix(const matrix_data& data, ostream& out = cout) {
  out << "Matrix: " << endl;
  out << data.rows << " " << data.cols << endl;

  for (int i = 0; i < data.rows; i++) {
    for (int j = 0; j < data.cols; j++) {
      out << data.matrix[i][j] << " ";
    }
    out << endl;
  }

  out << data.sonds_count << endl;

  for (int i = 0; i < data.sonds_count; i++) {
    out << data.radiuses_sonds[i] << " ";
  }
  out << endl;
}

void print_matrix(const vector<vector<int>>& data, ostream& out = cout) {
  string result = "";

  for (const auto& row : data) {
    for (int cell : row) {
      result += to_string(cell) + " ";
    }
    result += "\n";
  }
  result += "------------------------------------------------------";

  out << result << endl;
}

bool in_size(int coord, int max_size) { return coord >= 0 && coord < max_size; }

bool in_evklid(int dx, int dy, int radius) {
  return (dx * dx + dy * dy) <= radius * radius;
}

result back_tracking(matrix_data& data, otladka& sonds_cords, int depth) {
  if (depth == data.sonds_count) {
    return make_tuple(0, 0, 0);
  }

  int max_covered = 0;
  int max_sum = 0;
  int min_sensor_height_sum = 0;
  int radius = data.radiuses_sonds[depth];

  // pair<int, int> sond_coord = {-1, -1};

  for (auto it = value_coords.rbegin(); it != value_coords.rend(); ++it) {
    auto height = it->first;
    auto coordinates = it->second;

    if (coordinates.empty()) continue;

    for (auto& [y, x] : coordinates) {
      
      if (plain_matrix[y][x] == SOND) continue;

      vector<pair<int, int>> covered_cells;
      int covered_count = 0;
      int height_sum = 0;

      for (int dy = -radius; dy <= radius; dy++) {
        int ny = y + dy;

        if (!in_size(ny, data.rows)) continue;

        for (int dx = -radius; dx <= radius; dx++) {
          int nx = x + dx;

          if (!in_size(nx, data.cols) || !in_evklid(dx, dy, radius) ||
              (dx == x && dy == y))
            continue;

          int step_y = get_step(dy);  // Шаг по вертикали
          int step_x = get_step(dx);  // Шаг по горизонтали

          int next_x = nx + step_x;
          int next_y = ny + step_y;

          bool next_is_sond = next_x == x && next_y == y;

          if (next_is_sond) {
            if (plain_matrix[ny][nx] == EMPTY) {
              covered_count++;
              height_sum += data.matrix[ny][nx];
              plain_matrix[ny][nx] = COVERED;
              covered_cells.emplace_back(ny, nx);
            }
          } else if (data.matrix[ny][nx] >= data.matrix[next_y][next_x] &&
                     !next_is_sond) {
            if (plain_matrix[ny][nx] == EMPTY) {
              covered_count++;
              height_sum += data.matrix[ny][nx];
              plain_matrix[ny][nx] = COVERED;
              // covered_cells.emplace_back(ny, nx);
            }
          }

          // Идем по пути от текущей клетки (зонд) к целевой

          // Если сигнал заблокирован, пропускаем клетку

          // Если клетка пуста, увеличиваем покрытие
        }
      }

      if (plain_matrix[y][x] == EMPTY) {
        covered_count++;
        height_sum += height;
        // plain_matrix[y][x] = COVERED;
        covered_cells.emplace_back(y, x);
      }

      plain_matrix[y][x] = SOND;
      // sonds_cords[depth] = {radius, {y, x}};
      

      auto [sub_covered, sub_height_sum, sub_sensor_height] =
          back_tracking(data, sonds_cords, depth + 1);

      // Откат изменений
      // cout << covered_cells << endl;

      // plain_matrix[y][x] = EMPTY;
      for (auto& [cy, cx] : covered_cells) {
        // cout << "(" << cy << "," << cx << ") ";
        plain_matrix[cy][cx] = EMPTY;
      }
      // cout << endl;

      int total_covered = covered_count + sub_covered;
      int total_height_sum = height_sum + sub_height_sum;
      int total_sensor_sum = height + sub_sensor_height;
      
      if (total_covered > max_covered ||
          (total_covered == max_covered && total_height_sum > max_sum) ||
          (total_covered == max_covered && total_height_sum == max_sum &&
           total_sensor_sum < min_sensor_height_sum)) {
        max_covered = total_covered;
        max_sum = total_height_sum;
        min_sensor_height_sum = total_sensor_sum;

        sonds_cords[depth] = {radius, {y, x}};

        // cout << "New cord: "
      }
    }
  }

  return {max_covered, max_sum, min_sensor_height_sum};
}

int get_step(int dy) {
  if (dy == 0) {
    return 0;
  }

  return (dy < 0 ? 1 : -1);
}
