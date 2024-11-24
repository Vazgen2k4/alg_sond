#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

enum cell_state { EMPTY = 0, SOND = 1, COVERED = 2 };

typedef pair<int, int> Coord;
typedef vector<vector<int>> Matrix;
typedef tuple<int, int, int> BacktrackResult;
typedef map<int, vector<Coord>> CoordsMap;
// под вопросом
typedef map<int, pair<int, Coord>> otladka;

typedef struct Sond {
  int x;
  int y;
  int radius;

  Sond(int x, int y, int radius) : x(x), y(y), radius(radius) {}
  Sond() : x(-1), y(-1), radius(0) {}
  Sond(int radius) : x(-1), y(-1), radius(radius) {}
} Sond;

class MatrixData {
 public:
  void readInput(istream& in = cin);
  BacktrackResult backtrack(int depth = 0);
  void printSonds(ostream& out = cout);
  void printMatrixData(ostream& out = cout);
  void printMatrix(Matrix& matrix, ostream& out = cout);

 private:
  int rows = 0;
  int cols = 0;

  Matrix coatedMatrix = {};
  Matrix matrix = {};

  vector<Sond> sondsList = {};

  CoordsMap coordsMap = {
      {9, {}}, {8, {}}, {7, {}}, {6, {}}, {5, {}},
      {4, {}}, {3, {}}, {2, {}}, {1, {}},
  };

  Coord calculateSteps(Coord delta);
  bool isWithinBounds(int coord, int maxSize);
  bool isWithinEuclideanRadius(int dx, int dy, int radius);
  bool isBlocked(Coord curent, Coord next);

  void positionProcessing(Coord coord, Sond& sond,
                          function<void(int, int)> onComplete);
                          
  bool isPositionBetter(Coord covering, Coord sums, Coord sondSums);
};
