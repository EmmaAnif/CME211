#ifndef HEAT_HPP
#define HEAT_HPP

#include "CGSolver.hpp"
#include <cmath>
#include "COO2CSR.hpp"
#include <fstream>
#include <iostream>
#include "sparse.hpp"
#include <stdexcept>
#include <string>
#include <vector>

class HeatEquation2D
{
  private:
    SparseMatrix A;
    std::vector<double> b, x;

  public:
    /* Method to setup Ax=b system */
    int Setup(std::string inputfile);

    /* Method to solve system using CGsolver */
    int Solve(std::string soln_prefix);

};

#endif /* HEAT_HPP */
