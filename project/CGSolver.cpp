#include "CGSolver.hpp"
#include "matvecops.hpp"

/* Function that implements the CG algorithm for a tridiagonal block linear system
 *
 * Ax = b
 *
 * where A is in CSR format.  The starting guess for the solution
 * is provided in x, and the solver runs a maximum number of iterations
 * equal to the size of the linear system.  Function returns the
 * number of iterations to converge the solution to the specified
 * tolerance, or -1 if the solver did not converge. It also outputs a series
 * of solution files containing the solution after every 10 iterations including
 * the first and last iteration.
 * n_rows_block is the number of rows in each block of the tridiagonal matrix
 * soln_prefix is the solution prefix for the solution files*/

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol,
             int                 n_rows_block,
             std::string         soln_prefix)
{
    //get number of rows
    int n_row = (int)row_ptr.size() - 1;
    //get size of b vector
    int b_size = (int)b.size(); 
    
    //declare and initialize relevant variables
    int niter, nitermax = n_row, retVal = -1; //value for retval if no covergence
    std::vector<double> r, r_last, p, Ax, Ap;
    double alpha,beta, L2n0, L2n;
    
    Ax = matVecProduct(val,row_ptr,col_idx,x); //matrix vector multiply
    r = sum2Vec(b,scalVecProduct(Ax, -1)); //addition of vectors
        
    L2n0 = L2norm(r); //get L2 norm of vector
    p = r;
    niter = 0;
    int sol_num = 0; std::string outfile;
    
    //solve the equation using the CG algorithm
    while (niter < nitermax){
        niter++;
        r_last = r;
        
        Ap = matVecProduct(val,row_ptr,col_idx,p);
        alpha = dotProduct(r,r)/dotProduct(p,Ap); //dot product of two vectors
        
        x = sum2Vec(x,scalVecProduct(p,alpha)); //modify x vector in place
        r = sum2Vec(r,scalVecProduct(Ap,-alpha));
        
        L2n = L2norm(r);
        
        //check if solution has converged
        if ((L2n / L2n0) < tol){
            retVal = niter; //return number of iterations for convergence
            
            //write last solution file
            outfile = soln_prefix + std::to_string(sol_num) + ".txt";
            std::ofstream f(outfile);
            
            if (f.is_open()){
                for (int i = 0; i < b_size; i++){
                    if (i % n_rows_block == 0) //first row in block
                        f << b[i] << " ";
                    f << x[i] << " ";
                    if (i % n_rows_block == n_rows_block-1) //last row in block
                        f << b[i] << std::endl;
                }
            }
            else
                throw std::runtime_error("cannot open " + outfile);
            f.close();
            
            break;
        }
        
        beta = dotProduct(r,r)/dotProduct(r_last,r_last);
        p = sum2Vec(r,scalVecProduct(p,beta));
        
        //write solution file for intervals of 10 iterations
        if (niter % 10 == 1){
            outfile = soln_prefix + std::to_string(sol_num++) + ".txt";
            std::ofstream f(outfile);
            
            if (f.is_open()){
                for (int i = 0; i < b_size; i++){
                    if (i % n_rows_block == 0)
                        f << b[i] << " ";
                    f << x[i] << " ";
                    if (i % n_rows_block == n_rows_block-1)
                        f << b[i] << std::endl;
                }
            }
            else
                throw std::runtime_error("cannot open " + outfile);
            f.close();
        }
    }
    return retVal;
}

//--functionality_0
//--Passed all tests.
//--END
