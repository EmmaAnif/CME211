#include <fstream>
#include <iostream>
#include <vector>

#include "CGSolver.hpp"
#include "COO2CSR.hpp"

int main(){

    std::vector<double> val, b, x;
    std::vector<int> row_idx, col_idx, row_ptr;
    double tol = 1.e-5, value;
    int CGResult, n_row, n_col, row_ind, col_ind;
    
    std::ifstream f;
    
    //read the COO  matrix from the file and store in vectors
    f.open("matrix2.txt");
    if (f.is_open()){
        f >> n_row >> n_col;
        while (f >> row_ind >> col_ind >> value){
            val.push_back(value);
            row_idx.push_back(row_ind);
            col_idx.push_back(col_ind);
        }
        f.close();
    }
    else{
        std::cerr << "ERROR: Failed to open file" << std::endl;
    }
    
    /**
    std::cout << "n_row = " << n_row << std::endl;
    std::cout << "n_col = " << n_col << std::endl;
    
    std::cout << "values are: ";
    for (auto v: val){
    std::cout << v << " " ;
    }
    std::cout << std::endl;
    
    std::cout << "row indices are: ";
    for (auto v: row_idx){
    std::cout << v << " " ;
    }
    std::cout << std::endl;
    **/
    
    //convert the COO matrix to CSR matrix
    COO2CSR(val, row_idx, col_idx);
    row_ptr = row_idx;
    
    //using the equation Ax = b, define b and initialize x
    b.assign(n_row,0);
    x.assign(n_row,1);
    
    /**********
    std::cout << "b: ";
    for (auto v: b){
    std::cout << v << " " ;
    }
    std::cout << std::endl;

    std::cout << "x: ";
    for (auto v: x){
    std::cout << v << " " ;
    }
    std::cout << std::endl;

    return 0;
    *********/
    
    CGResult = CGSolver(val, row_ptr, col_idx, b, x, tol);
    
    if (CGResult != -1){
        std::cout << "SUCCESS: CG solver converged in ";
        std::cout << CGResult << " iteration(s)" << std::endl;
    }
    else{
        std::cout << "FAILURE: CG Solver failed to converge" << std::endl;
    } 
    
    return 0;
}
