
#pragma once

#include <vector>

using namespace std;

#define TYPE long double // ��� ������ ��� ���������� 
#define ACCURACY 1e-5 // �������� ���������� 
#define BUFF_READ_SIZE 64
// #define WITH_ZERO_DIVISION_CHECK // ��������� ������� �� ���� 
// #define SIMPLE_NUMBERS // �������������� ����� ����� 0..10 � ��������

namespace matrix_processing {
 
  // ������ ������ � ������������ ��������� � ������� i
  int findMaxInColumn(const vector<vector<TYPE>> &linear_system, int i_col, int i_from = 0);

  // �������� ������� 2 ������ ������� 
  void swapRows(vector<vector<TYPE>> &linear_system, int i_from, int i_to);

  // �������� ��� �������� ������ �� ����� 
  void divideRowBy(vector<vector<TYPE>> &linear_system, int i_row, TYPE x);

  // ��������� ����� �������
  void subRows(vector<vector<TYPE>> &linear_system, int i_row, int i_row_arg);

  // �������� ������ ������� �� ������
  TYPE multiplyRowBy(const vector<vector<TYPE>> &linear_system, const int i, const vector<TYPE> &x);

  // ������ ������
  void printMatrix(const vector<vector<TYPE>> &linear_system);

  // ������������� ��������� ���� 
  vector<vector<TYPE>> generateLinearSystem(int n, int m, bool small_numbers = false);

  // ������� ������� �� ����� 
  vector<vector<TYPE>> dowloadlinear_system(const std::string &filePath);

  // ����� �������
  void printVector(const vector<TYPE> &v);
}
