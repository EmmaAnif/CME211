#include "sparse.hpp"

/* Method to modify sparse matrix dimensions */
void SparseMatrix::Resize(int nrows, int ncols){
  this->i_idx.resize(nrows);
  this->j_idx.resize(ncols);
  this->a.resize(nrows);
}

/* Method to add entry to matrix in COO format */
void SparseMatrix::AddEntry(int i, int j, double val){
  this->i_idx.push_back(i);
  this->j_idx.push_back(j);
  this->a.push_back(val);
}

/* Method to convert COO matrix to CSR format using provided function */
void SparseMatrix::ConvertToCSR(){
  COO2CSR(this->a, this->i_idx, this->j_idx);
}

/* Method to perform sparse matrix vector multiplication using CSR formatted matrix */
std::vector<double> SparseMatrix::MulVec(std::vector<double> &vec){
  return matVecProduct(this->a,this->i_idx,this->j_idx,vec);
}
