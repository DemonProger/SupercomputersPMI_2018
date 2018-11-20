
#pragma once

#include <vector>

#define TYPE long double // ��� ������ ��� ���������� 
#define ACCURACY 1e-2 // �������� ���������� 
#define BUFF_READ_SIZE 64
// #define WITH_ZERO_DIVISION_CHECK // ��������� ������� �� ���� 
// #define SIMPLE_NUMBERS // �������������� ����� ����� 0..10 � ��������

using namespace std;

namespace consistent_gauss {
  // ���������� ������� � ������� �����������
  void toUpperTriangular(vector<vector<TYPE>> &linear_system);

  // ������� ���� � �������� ���������� ����������� � �������� ������������ ���� 
  vector<TYPE> solveUppertriangular(const vector<vector<TYPE>> &linear_system);

  // �������� ������������ ������� ������� ���� 
  bool checkSolution(const vector<vector<TYPE>> &linear_system, const vector<TYPE> &x, const bool withPrint = false);

  // ����� ������������������ ����������������� ������� 
  vector<pair<int, double>> consistentSolutionExperiment(int m_size_from, int m_size_to, int step, bool with_print = false);
}