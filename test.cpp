#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#define TEST
#include "v1.cpp"

namespace fs = std::filesystem;
void test(const std::string &input_file, const std::string &expect_file,
          std::ostream &out);

void tests_from_file(const std::string &foldernamee, std::ostream &out);

int main() {
  std::ofstream file("test.out", std::ios::out);
  tests_from_file("tests", file);
  file.close();
  return 0;
}

void test(const std::string &input_file, const std::string &expect_file,
          std::ostream &out = cout) {
  std::ifstream input(input_file);
  std::ifstream expected_output(expect_file);

  if (!input.is_open() || !expected_output.is_open()) {
    std::cerr << "Error: Unable to open files: " << input_file << " or "
              << expect_file << std::endl;
    return;
  }

  matrix_data data;
  otladka coord = {};
  // vector<vector<int>> status(data.rows, vector<int>(data.cols, EMPTY));

  read_input(data, input);

  int a, b, c;
  expected_output >> a >> b >> c;

  auto [a_res, b_res, c_res] = back_tracking(data, coord);

  if (a == a_res && b == b_res && c == c_res) {
    out << "Test passed!" << endl;
  } else {
    out << "Test failed!" << endl;
    out << "Input: " << endl;

    print_matrix(data, out);

    out << "Expected Output: " << a << " " << b << " " << c << endl;
    out << "Output: " << a_res << " " << b_res << " " << c_res << endl;

    for (auto &[key, val] : coord) {
      out << "Вышка №<" << key << ">: (" << val.second.first << ", "
          << val.second.second << "), дальность: " << val.first << endl;
    }
  }
  out << "------------------------------------------------------" << endl;

  assert(a == a_res && b == b_res && c == c_res);
}

void tests_from_file(const std::string &foldername,
                     std::ostream &out = std::cout) {
  try {
    for (const auto &entry : fs::directory_iterator(foldername)) {
      if (entry.is_regular_file() && entry.path().extension() == ".in") {
        fs::path input_file = entry.path();
        fs::path output_file = input_file;
        output_file.replace_extension(".out");

        if (fs::exists(output_file)) {
          test(input_file.string(), output_file.string(), out);
        } else {
          std::cerr << "Warning: No corresponding .out file for " << input_file
                    << std::endl;
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
