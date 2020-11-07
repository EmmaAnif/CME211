#include "matvecops.hpp"

/******************************************************************************
Functions that perform common matrix vector operations that occur in the 
CG algorithm
* L2norm
* dotProduct
* matVecProduct
* scalVecProduct
* sum2Vec
******************************************************************************/

/* L2norm
* 
* Parameters: std::vector<double> r_n
* Returns: double return_L2norm
*
* Description: calculates the L2norm of a vector */
double L2norm(const std::vector<double> &r_n){
    std::vector<double> squareVec;
    for (auto r: r_n){
        squareVec.push_back(std::pow(r,2.0));
    }
    
    double sumSquareVec = std::accumulate(squareVec.begin(),squareVec.end(), 0.0);
    double return_L2norm = std::sqrt(sumSquareVec);
    
    return return_L2norm; 
}

/* dotProduct
* 
* Parameters: std::vector<double> r_m, r_n
* Returns: double sumProdVec
*
* Description: calculates the dot product of two vectors */
double dotProduct(const std::vector<double> &r_m,
                const std::vector<double> &r_n){
    std::vector<double> prodVec;
    for (int i = 0; i < (int)r_m.size(); i++){
        prodVec.push_back(r_m[i] * r_n[i]);
    }
    double sumProdVec = std::accumulate(prodVec.begin(),prodVec.end(),0.0);
    return sumProdVec;
}

/* matVecProduct
* 
* Parameters: std::vector<double> val, row_ptr, col_idx, r_n
* Returns: std::vector<double> resultVec
*
* Description: calculates the matrix vector product of a CSR matrix represented
* by val, row_ptr, col_idx and a vector r_n  */
std::vector<double> matVecProduct(const std::vector<double> &val,
                    const std::vector<int> &row_ptr,
                    const std::vector<int> &col_idx,
                    const std::vector<double> &r_n){
    
    int n_row = (int)r_n.size();
    std::vector<double> resultVec(n_row,0);
    
    for (int i = 0; i < n_row; i++){
        for (int k = row_ptr[i]; k < row_ptr[i+1]; k++){
            resultVec[i] += val[k] * r_n[col_idx[k]];
        } 
    }
    return resultVec;
}

/* scalVecProduct
* 
* Parameters: std::vector<double> r_n, double k
* Returns: std::vector<double> resultVec
*
* Description: calculates the product of a vector and a scalar */
std::vector<double> scalVecProduct(const std::vector<double> &r_n, double k){
    std::vector<double> resultVec;
    for (auto r: r_n){
        resultVec.push_back(r * k);
    }
    return resultVec;
}

/* sum2Vec
* 
* Parameters: std::vector<double> r_m, r_n
* Returns: std::vector<double> resultVec
*
* Description: calculates the sum of two vectors */
std::vector<double> sum2Vec(const std::vector<double> &r_m, const std::vector<double> &r_n){
    std::vector<double> resultVec;
    for (int i = 0; i < (int)r_m.size(); i++){
        resultVec.push_back(r_m[i] + r_n[i]);
    }
    return resultVec;
}
