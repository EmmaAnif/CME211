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
 * tolerance, or -1 if the solver did not converge.*/

int CGSolver(std::vector<double> &val,
             std::vector<int>    &row_ptr,
             std::vector<int>    &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double              tol)
{
    //get number of rows
    int n_row = (int)row_ptr.size() - 1; 
    
    //declare and initialize relevant variables
    int niter, nitermax = n_row, retVal = -1; //value for retval if no covergence
    std::vector<double> r, r_last, p, Ax, Ap;
    double alpha,beta, L2n0, L2n;
    
    Ax = matVecProduct(val,row_ptr,col_idx,x); //matrix vector multiply
    r = sum2Vec(b,scalVecProduct(Ax, -1)); //addition of vectors
        
    L2n0 = L2norm(r); //get L2 norm of vector
    p = r;
    niter = 0;
    while (niter < nitermax){
        niter++;
        r_last = r;
        
        Ap = matVecProduct(val,row_ptr,col_idx,p);
        alpha = dotProduct(r,r)/dotProduct(p,Ap); //dot product of two vectors
        
        x = sum2Vec(x,scalVecProduct(p,alpha)); //modify x vector in place
        r = sum2Vec(r,scalVecProduct(Ap,-alpha));
        
        L2n = L2norm(r);
        
        if ((L2n / L2n0) < tol){ //check if ratio of norms is within threshold
            retVal = niter; //return number of iterations for convergence
            break;
        }
        
        beta = dotProduct(r,r)/dotProduct(r_last,r_last);
        p = sum2Vec(r,scalVecProduct(p,beta));
    }
    return retVal;
}

//--functionality_0
//--Passed all tests!
//--END

//--design_0
//--Well designed!
//--END

//--documentation_0
//--good
//--END


