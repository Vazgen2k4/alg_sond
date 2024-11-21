#pragma once
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

typedef tuple<int, int, int> rec_result;

vector<vector<int>> plain_matrix;
// {vakue, {{x1, y1}, {x2, y2}...}}
map<int, vector<pair<int, int>>> value_coords;

void read_input(matrix_data& data, vector<int>& values_sonds,
                istream& in = cin);

rec_result back_tracking(matrix_data& data, vector<int>& values_sonds);

#ifndef TEST

int main(int argc, char const* argv[]) {
  matrix_data data;

  vector<int> values_sonds;
  vector<pair<int, int>> precounted_sonds_sums;
  read_input(data, values_sonds);

  return 0;
}
#endif

void precount_sond_min_max_sum(int sond_val) {
  int res = 1;  // sond point
}

void read_input(matrix_data& data, vector<int>& values_sonds, istream& in) {
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

rec_result back_tracking(matrix_data& data, vector<int>& values_sonds) {
  
  return rec_result(19, 19, 19);
}
