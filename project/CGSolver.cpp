#include "CGSolver.hpp"
#include "matvecops.hpp"

/* Function that implements the CG algorithm for a linear system
 *
 * Ax = b
 *
 * where A is in CSR format.  The starting guess for the solution
 * is provided in x, and the solver runs a maximum number of iterations
 * equal to the size of the linear system.  Function returns the
 * number of iterations to converge the solution to the specified
 * tolerance, or -1 if the solver did not converge.
 */

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol)
{
    //get number of rows and columns
    int n_row = (int)row_ptr.size() - 1; 
    
    /****** , n_col = (int)col_idx.size();
    std::cout << "n_row = " << n_row << std::endl;
    std::cout << "n_col = " << n_col << std::endl;
    if (n_row != n_col){
        std::cout << "ERROR: Improper system of equations" << std::endl;
        return -1;
    }
    *********/
    
    //store relevant variables
    int niter, nitermax = n_row, retVal = -1;
    std::vector<double> u, r, r_last, p, Au, Ap;
    double alpha,beta, L2n0, L2n;
    
    u = x;
    Au = matVecProduct(val,row_ptr,col_idx,u); //matrix vector multiply
    r = sum2Vec(b,scalVecProduct(Au, -1));
        
    L2n0 = L2norm(r);
    p = r;
    niter = 0;
    while (niter < nitermax){
        niter++;
        r_last = r;
        
        /*std::cout << "r_n: ";
        for (auto v: r_last){
            std::cout << v << " ";
        } 
        std::cout << std::endl;*/

        Ap = matVecProduct(val,row_ptr,col_idx,p);
        alpha = dotProduct(r,r)/dotProduct(p,Ap);
        //std::cout << "alpha: " << alpha << std::endl;
        
        u = sum2Vec(u,scalVecProduct(p,alpha));
        r = sum2Vec(r,scalVecProduct(Ap,-alpha));
        
        /*std::cout << "r_n+1: ";
        for (auto v: r){
            std::cout << v << " ";
        } 
        std::cout << std::endl;*/
        
        L2n = L2norm(r);
        
        /*std::cout << "L2n: " << L2n << std::endl;
        std::cout << "L2n0: " << L2n0 << std::endl;
        std::cout << "L2norm ratio: " << (L2n/L2n0) << std::endl;
        std::cout << "tol: " << tol << std::endl << std::endl;*/
        
        if ((L2n / L2n0) < tol){
            retVal = niter;
            break;
        }
        
        beta = dotProduct(r,r)/dotProduct(r_last,r_last);
        //std::cout << "beta: " << beta << std::endl;
        p = sum2Vec(r,scalVecProduct(p,beta));
    }
    return retVal;
}
