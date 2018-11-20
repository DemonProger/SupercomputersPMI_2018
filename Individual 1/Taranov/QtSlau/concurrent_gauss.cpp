
#include "concurrent_gauss.h"
#include "matrix_proc.h"
#include <map>

#include <omp.h>

using namespace matrix_processing;

#define N_THREADS 4

// ���������� ������� � ������� �����������
void concurrent_gauss::toUpperTriangular(vector<vector<TYPE>>& linear_system) {
  for (int j = 0; j < linear_system.size() - 1; ++j) {

    // �������� ���������� ������� � ������� ������ ��� ������ � j-�� �������
    int max_row = findMaxInColumn(linear_system, j, j);
    swapRows(linear_system, j, max_row);

#pragma omp parallel for shared(linear_system) num_threads(N_THREADS) 
      // ��������� ������ �� j-�� �������� 
      for (int i = j; i < linear_system.size(); ++i)
        if (linear_system[i][j] != 0)
          divideRowBy(linear_system, i, linear_system[i][j]);
    

#pragma omp parallel for shared(linear_system) num_threads(N_THREADS) 
      // �������� j-�� ������ �� ���� ��������� 
      for (int i = 0; i < linear_system.size(); ++i)
        if (i != j)
          subRows(linear_system, i, j);    
    
  }
}

// ������� ���� � �������� ���������� ����������� � �������� ������������ ���� 
vector<TYPE> concurrent_gauss::solveUppertriangular(const vector<vector<TYPE>>& linear_system) {
  vector<TYPE> x(linear_system.size(), 0);

  int last_row = linear_system.size() - 1;
  int last_col = linear_system[0].size() - 1;


  x[last_row] = linear_system[last_row][last_col] / linear_system[last_row][last_col - 1];

  for (int i = last_row - 1; i >= 0; --i) {

    // ����� ��������� ������ �� ������������� (����� ���������� �������) � ������ ��������� �
    TYPE sum = 0;
#pragma omp parallel for shared(linear_system) reduction(+: sum)
    for (int j = i + 1; j <= last_col - 1; ++j)
      sum += linear_system[i][j] * x[j];

    x[i] = (linear_system[i][last_col] - sum) / linear_system[i][i];
  }

  return x;
}

// �������� ������������ ������� ������� ���� 
bool concurrent_gauss::checkSolution(const vector<vector<TYPE>>& linear_system, const vector<TYPE>& x, const bool withPrint) {
  for (int i = 0; i < linear_system.size(); ++i) {
    TYPE m = multiplyRowBy(linear_system, i, x);
    TYPE m_right = linear_system[i][linear_system[0].size() - 1];

    if (abs(m - m_right) > ACCURACY) { // mistake was found
      if (withPrint)
        cout << "Mistake was found in row " << i << " " << m << " != " << m_right << "\n";

      return false;
    }
    else { // current checking is succeed
      cout << m << " = " << m_right << " ok\n";
    }
  }

  if (withPrint)
    cout << "Solution is write\n";

  return true;
}

// ����� ������������������ ����������������� ������� 
vector<pair<int, double>> concurrent_gauss::concurrentSolutionExperiment(int m_size_from, int m_size_to, int step, bool with_print) {
  vector<pair<int, double>> res;

  for (int i = m_size_from; i <= m_size_to; i += step) {

    // generation of source linear system
#ifdef SIMPLE_NUMBERS
    auto linear_system = generateLinearSystem(i, i + 1, true);
#else
    auto linear_system = generateLinearSystem(i, i + 1, false);
#endif  

    // print
    //if (with_print) {
    //  cout << "Generated linear system with size " << i << " x " << i + 1 << '\n';
    //  printMatrix(linear_system);
    //}

    try {
        // solution
        double ms_start = omp_get_wtime();
        toUpperTriangular(linear_system);
        //if (with_print) {
        //  cout << "Upper triangular:\n";
        //  printMatrix(linear_system);
        //}
        auto v_solution = solveUppertriangular(linear_system);
        double ms_time = omp_get_wtime() - ms_start;

        // print 
        if (with_print) {
          cout << "Solution: ";
          printVector(v_solution);

          cout << "Time = " << ms_time << endl;

          checkSolution(linear_system, v_solution, true);
        }

        res.push_back({ i, ms_time });
      }
      catch (exception e) {
        res.push_back({ i, -1 });
      }    
  }

  return res;
}
