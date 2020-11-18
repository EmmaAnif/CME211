#include "heat.hpp"
#include <stdexcept>
#include <iostream>

double length, width, h, Tc, Th;
int nrows, ncols, m, n;

/* Method to setup Ax=b system */
int HeatEquation2D::Setup(std::string inputfile){
  std::ifstream f(inputfile);
  
  if (f.is_open()){
    f >> length >> width >> h;
    f >> Tc >> Th;
    f.close();
  }
  else{
    std::cout << "Could not open " << inputfile << std::endl;
    //spit out error message
  }
  
  nrows = (int)(width/h)+1;
  ncols = (int)(length/h)+1;
  m = nrows - 2, n = ncols - 1;
  
  /*std::cout << "row: " << nrows << " col: " << ncols << std::endl;
  std::cout << "Th: " << Th << " Tc: " << Tc << std::endl;*/
  
  //boundary conditions
  double y, val;
  std::vector<double> Tc_arr;
  for (int i = 0; i < ncols; i++){
    y = ((double)i/(double)n)*length;
    val = -Tc*(exp(-10*std::pow(y-(length/2),2))-2);
    Tc_arr.push_back(val);
  }
  /*std::cout << "Tc_arr" << std::endl;
  for (auto T: Tc_arr)
    std::cout << T << std::endl;
  
  throw std::runtime_error(" ");*/
  
  //Build A matrix in COO format
  //std::cout << "A Matrix in COO format" << std::endl;
  for (int i = 0; i < m*n; i++){
    for (int j = 0; j < m*n; j++){
      if (i == j) //diagonal
        A.AddEntry(i, j, 4);
      else if (i-j == 1 and i%m != 0) //sub diagonal
        A.AddEntry(i, j, -1);
      else if (j-i == 1 and j%m != 0) //sup diagonal
        A.AddEntry(i, j, -1);
      else if (j-i == m) //sup sup diagonal
        A.AddEntry(i, j, -1);
      else if (i-j == m) //sub sub diagonal
        A.AddEntry(i, j, -1);
      else if (abs(i-j) == m*(n-1)) //periodic boudary 
        A.AddEntry(i, j, -1);
      /*else if (j-1 == m*(n-1)) //periodic boudary sup
        A.AddEntry(i, j, -1);*/
    }
  }
 /* for (int i = 0; i < (int)A.a.size(); i++){
    std::cout << A.i_idx[i] << "  " << A.j_idx[i] << "  " << A.a[i] << std::endl;
  }
  std::cout << std::endl << "m: " << m << " n: " << n << std::endl;
  //throw std::runtime_error("wahala for bug wey still dey here oo");*/
  
  //Build x vector
  int b_idx = 0; //initialize cold boundary index
  for (int i = 0; i < m*n; i++){
    if (i%m == 0)
      b.push_back(Th);
    else if (i%m == m-1)
      b.push_back(Tc_arr[b_idx++]);
    else
      b.push_back(0);
      
    x.push_back(0); //initialize x with zeros
  }
  
  return 0;
}

/* Method to solve system using CGsolver */
double tol = 1.e-5; int CGResult;
int HeatEquation2D::Solve(std::string soln_prefix){
  COO2CSR(A.a, A.i_idx, A.j_idx);
  CGResult = CGSolver(A.a, A.i_idx, A.j_idx, b, x, tol, m, soln_prefix);
  if (CGResult != -1){
    std::cout << "SUCCESS: CG solver converged in ";
    std::cout << CGResult << " iteration(s)." << std::endl;
  }
  else{
    std::cerr << "FAILURE: CG Solver failed to converge." << std::endl;
  }
  
  return 0;
}
