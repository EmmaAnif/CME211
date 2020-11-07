#ifndef MATVECOPS_HPP
#define MATVECOPS_HPP

#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

/******************************************************************************
Functions that perform common matrix vector operations that occur in the 
CG algorithm
* L2norm
* dotProduct
* matVecProduct
* scalVecProduct
* sum2Vec
******************************************************************************/

//function prototypes
double L2norm(const std::vector<double> &r_n);
double dotProduct(const std::vector<double> &r_m,
                const std::vector<double> &r_n);
std::vector<double> matVecProduct(const std::vector<double> &val,
                    const std::vector<int> &row_ptr,
                    const std::vector<int> &col_idx,
                    const std::vector<double> &r_n);
std::vector<double> scalVecProduct(const std::vector<double> &r_n, double k);
std::vector<double> sum2Vec(const std::vector<double> &r_m, const std::vector<double> &r_n);
                    
#endif //MATVECOPS_HPP
