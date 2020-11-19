#ifndef CGSOLVER_HPP
#define CGSOLVER_HPP

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

/* Function that implements the CG algorithm for a tridiagonal block linear system
 *
 * Ax = b
 *
 * where A is in CSR format.  The starting guess for the solution
 * is provided in x, and the solver runs a maximum number of iterations
 * equal to the size of the linear system.  Function returns the
 * number of iterations to converge the solution to the specified
 * tolerance, or -1 if the solver did not converge. It also outputs a series of 
 * solution files containing the solution after every 10 iterations including
 * the first and the last iteration.
 * n_rows_block is the number of rows in each block of the tridiagonal matrix
 * soln_prefix is the solution prefix for the solution files*/

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol,
             int                 n_rows_block,
             std::string         soln_prefix);

#endif /* CGSOLVER_HPP */
