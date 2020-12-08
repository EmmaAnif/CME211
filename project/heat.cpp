#include "heat.hpp"

//declare relevant variables
double length, width, h, Tc, Th;
int nrows, ncols, m, n;

/* Method to setup Ax=b system */
int HeatEquation2D::Setup(std::string inputfile){
  std::ifstream f(inputfile);
  
  //open the input file and store relevant data
  if (f.is_open()){
    f >> length >> width >> h;
    f >> Tc >> Th;
    f.close();
  }
  else{
    throw std::runtime_error("cannot open " + inputfile);
  }
  
  //store number of rows and columns
  // m*n gives the dimension of the block tridiagonal matrix used to set up system
  //m is the number of rows in each block of the matrix
  //n is the number of block rows in the matrix
  nrows = (int)(width/h)+1;
  ncols = (int)(length/h)+1;
  m = nrows - 2, n = ncols - 1;
  
  //store boundary conditions
  double y, val;
  std::vector<double> Tc_arr;
  for (int i = 0; i < ncols; i++){
    y = ((double)i/(double)n)*length;
    val = -Tc*(exp(-10*std::pow(y-(length/2),2))-2);
    Tc_arr.push_back(val);
  }
  
  //Build A matrix in COO format
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
      else if (abs(i-j) == m*(n-1)) //periodic boudary condition
        A.AddEntry(i, j, -1);
    }
  }
  
  //Build b vector and initialize x vector with zeros
  int b_idx = 0; //initialize cold boundary index
  for (int i = 0; i < m*n; i++){
    if (i%m == 0) //first row in block
      b.push_back(Th);
    else if (i%m == m-1)//last row in block
      b.push_back(Tc_arr[b_idx++]);
    else
      b.push_back(0);
      
    x.push_back(0); //initialize x with zeros
  }
  
  return 0;
}

double tol = 1.e-5; int CGResult;

/* Method to solve system using CGsolver */
int HeatEquation2D::Solve(std::string soln_prefix){
  //convert COO matrix to CSR matrix
  A.ConvertToCSR();

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

//--functionality_0
//--Passed all tests.
//--END
