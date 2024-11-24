#include "hw.hpp"

using namespace std;

#ifndef TEST

int main(int argc, char const* argv[]) {
  MatrixData data;

  data.readInput();

  auto res = data.backtrack();

  cout << get<0>(res) << " " << get<1>(res) << " " << get<2>(res) << endl;

  return 0;
}

#endif

BacktrackResult MatrixData::backtrack(int depth) {
  if (depth == sondsList.size()) {
    return make_tuple(0, 0, 0);
  }

  auto& sond = sondsList[depth];
  int maxCovered = 0;
  int maxSum = 0;
  int minSondHeightSum = numeric_limits<int>::max();

  for (auto it = coordsMap.rbegin(); it != coordsMap.rend(); ++it) {
    auto height = it->first;
    auto coordinates = it->second;

    if (coordinates.empty()) continue;

    for (auto& [y, x] : coordinates) {
      // если на текущей клетке зонд, пропускаем клетку
      if (coatedMatrix[y][x] == SOND) continue;

      coatedMatrix[y][x] = SOND;
      vector<Coord> coveredCells;
      int heightSum = matrix[y][x];
      coveredCells.emplace_back(y, x);

      // Обработка клеток в радиусе зонда
      for (int dy = -sond.radius; dy <= sond.radius; dy++) {
        int ny = y + dy;

        if (!isWithinBounds(ny, rows)) continue;

        for (int dx = -sond.radius; dx <= sond.radius; dx++) {
          int nx = x + dx;

          if (!isWithinBounds(nx, cols) ||
              !isWithinEuclideanRadius(dx, dy, sond.radius) ||
              (nx == x && ny == y) || coatedMatrix[ny][nx] != EMPTY)
            continue;

          auto [step_y, step_x] = calculateSteps({dy, dx});

          int next_x = nx + step_x;
          int next_y = ny + step_y;

          bool next_is_sond = next_x == x && next_y == y;
          bool blocked = isBlocked({ny, nx}, {next_y, next_x});

          if (!next_is_sond && blocked) {
            continue;
          }

          heightSum += matrix[ny][nx];
          coatedMatrix[ny][nx] = COVERED;
          coveredCells.emplace_back(ny, nx);
        }
      }

      // Рекурсивный вызов
      auto [subCovered, subHeightSum, subSondHeight] = backtrack(depth + 1);

      // Текущие результаты после рекурсии
      int currentCovered = coveredCells.size() + subCovered;
      int currentHeightSum = heightSum + subHeightSum;
      int currentSondSum = height + subSondHeight;

      // Обновляем координаты и параметры, если текущий вариант лучший
      if (currentCovered > maxCovered ||
          (currentCovered == maxCovered && currentHeightSum > maxSum) ||
          (currentCovered == maxCovered && currentHeightSum == maxSum &&
           currentSondSum < minSondHeightSum)) {
        maxCovered = currentCovered;
        maxSum = currentHeightSum;
        minSondHeightSum = currentSondSum;

        sond.x = x;
        sond.y = y;

        cout << "Зонд [" << &sond << "] обновлен: (y:" << y << ", x:" << x
             << ")" << endl;

        cout << "Текущие данные: (" << maxCovered << ", " << maxSum << ", "
             << minSondHeightSum << ")\n";
        cout << "Лучшие покрытые клетки для зонда: " << endl;
        for (auto& [cy, cx] : coveredCells) {
          cout << "(y:" << cy << ", x:" << cx << ") ";
        }
        cout << endl << "----------------------------------------" << endl;
      }

      for (auto& [cy, cx] : coveredCells) {
        coatedMatrix[cy][cx] = EMPTY;
      }
    }
  }

  return {maxCovered, maxSum, minSondHeightSum};
}

void MatrixData::printSonds(ostream& out) {
  int i = 0;
  string result = "";

  for (auto const& sond : sondsList) {
    // cout << "Sond № (" << sond.radius << ")\n";

    result += "Sond №" + to_string(++i) + ": radius=[" +
              to_string(sond.radius) + "], cords=(y:" + to_string(sond.y) +
              ", x:" + to_string(sond.x) + ")\n";
  }

  out << result << endl;
}

void MatrixData::readInput(istream& in) {
  in >> rows >> cols;
  matrix.resize(rows, vector<int>(cols, 0));
  coatedMatrix.resize(rows, vector<int>(cols, 0));

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      in >> matrix[i][j];
      int val = matrix[i][j];
      coordsMap[val].push_back({i, j});

      cout << "(" << i << ", " << j << "): " << val << endl;
    }
  }
  int count;
  in >> count;
  sondsList.resize(count, Sond());

  for (auto& sond : sondsList) {
    in >> sond.radius;
  }

  for (auto& [key, value] : coordsMap) {
    cout << "key: " << key << ", value: ";
    for (auto& [y, x] : value) {
      cout << "(" << y << ", " << x << ") ";
    }
    cout << endl;
  }

  reverse(sondsList.begin(), sondsList.end());
}

Coord MatrixData::calculateSteps(Coord delta) {
  int dy = delta.first;
  int dx = delta.second;

  auto _calcStep = [](int dv) {
    if (dv == 0) {
      return 0;
    }

    return (dv < 0 ? 1 : -1);

    return 0;
  };

  return {_calcStep(dy), _calcStep(dx)};
}

void MatrixData::printMatrixData(ostream& out) {
  string result = "Matrix: \n" + to_string(rows) + " " + to_string(cols) + "\n";

  for (auto row : matrix) {
    for (auto cell : row) {
      result += to_string(cell) + " ";
    }
    result += "\n";
  }

  result += to_string(sondsList.size()) + "\n";

  for (auto sond : sondsList) {
    result += to_string(sond.radius) + " ";
  }
  out << result << endl;
}

void MatrixData::printMatrix(Matrix& matrix, ostream& out) {
  string result = "";

  for (const auto& row : matrix) {
    for (int cell : row) {
      result += to_string(cell) + " ";
    }
    result += "\n";
  }
  result += "------------------------------------------------------";
  out << result << endl;
}

bool MatrixData::isWithinBounds(int coord, int maxSize) {
  return coord >= 0 && coord < maxSize;
}

bool MatrixData::isWithinEuclideanRadius(int dx, int dy, int radius) {
  return sqrt(dx * dx + dy * dy) <= radius;
}

bool MatrixData::isBlocked(Coord current, Coord next) {
  return matrix[current.first][current.second] <
         matrix[next.first][next.second];
}
