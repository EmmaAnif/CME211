#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "CGSolver.hpp"
#include "COO2CSR.hpp"

int main(int argc, char *argv[]){

    //catch the case where insufficient arguments are provided
    if (argc < 3){
        std::cout << "Usage: " << std::endl;
        std::cout << " " << argv[0] << " <input matrix file name>";
        std::cout << " <output solution file name> " << std::endl;
        return 0;
    }
    
    //save name of matrix and solution files 
    std::string matrix_file = argv[1];
    std::string solution_file = argv[2];
    
    //declare relevant variables and initialize tolerance
    std::vector<double> val, b, x;
    std::vector<int> row_idx, col_idx, row_ptr;
    double tol = 1.e-5, value;
    int CGResult, n_row, n_col, row_ind, col_ind;
    
    //open matrix file
    std::ifstream f(matrix_file);
    
    //read the COO  matrix from the file and store in vectors
    if (f.is_open()){
        //save number of rows and columns
        f >> n_row >> n_col;
        //store COO matrix values in vectors
        while (f >> row_ind >> col_ind >> value){
            val.push_back(value);
            row_idx.push_back(row_ind);
            col_idx.push_back(col_ind);
        }
        f.close();
    }
    else{
        std::cerr << "ERROR: Failed to open " << matrix_file << std::endl;
        return 0;
    }
    
    //convert the COO matrix to CSR matrix
    COO2CSR(val, row_idx, col_idx);
    
    //using the equation Ax = b, define b and initialize x
    b.assign(n_row,0);
    x.assign(n_row,1);
    
    // solve equation using the CGSolver.. row_idx is the row pointer vector
    CGResult = CGSolver(val, row_idx, col_idx, b, x, tol);
    
    if (CGResult != -1){
        std::cout << "SUCCESS: CG solver converged in ";
        std::cout << CGResult << " iteration(s)." << std::endl;
    }
    else{
        std::cerr << "FAILURE: CG Solver failed to converge." << std::endl;
        return 0;
    } 
    
    //open solution file
    std::ofstream g(solution_file);
    
    //write the solution vector to solution file
    if (g.is_open()){
        g.setf(std::ios::scientific, std::ios::floatfield);
        g.precision(3);
        for (auto vec: x){
            g << vec << std::endl;
        }
        g.close();
    }
    else{
        std::cerr << "ERROR: failed to  open " << solution_file << std::endl;
        return 0;
    }
    
    return 0;
}
