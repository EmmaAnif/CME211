//Functions that perform common matrix vector operations that occur in the 
//CG algorithm

#include "matvecops.hpp"

double L2norm(const std::vector<double> &r_n){
    std::vector<double> squareVec;
    for (auto r: r_n){
        squareVec.push_back(std::pow(r,2.0));
    }
    /*
    std::cout << "s: ";
    for (auto s: squareVec){
        std::cout << s << " ";
    }
    std::cout << std::endl;*/
    
    double sumSquareVec = std::accumulate(squareVec.begin(),squareVec.end(), 0.0);
    //std::cout << "sumSquares: " << sumSquareVec << std::endl;
    double return_L2norm = std::sqrt(sumSquareVec);
    return return_L2norm;
    
}

double dotProduct(const std::vector<double> &r_m,
                const std::vector<double> &r_n){
    std::vector<double> prodVec;
    for (int i = 0; i < (int)r_m.size(); i++){
        prodVec.push_back(r_m[i] * r_n[i]);
    }
    double sumProdVec = std::accumulate(prodVec.begin(),prodVec.end(),0.0);
    return sumProdVec;
}

std::vector<double> matVecProduct(const std::vector<double> &val,
                    const std::vector<int> &row_ptr,
                    const std::vector<int> &col_idx,
                    const std::vector<double> &r_n){
    /**********
    std::cout << "Got inside matVecprod" << std::endl;
    std::cout << "val: ";
    for (auto v: val){
    std::cout << v << " ";
    }
    std::cout << std::endl;
    
    std::cout << "row_ptr: ";
    for (auto v:row_ptr){
    std::cout << v << " ";
    }
    std::cout << std::endl;
    
    std::cout << "col_idx: ";
    for (auto v:col_idx){
    std::cout << v << " ";
    }
    std::cout << std::endl;
    
    std::cout << "r_n: ";
    for (auto v:r_n){
    std::cout << v << " ";
    }
    std::cout << std::endl;
    **************/
    
    int n_row = (int)r_n.size();
    //std::cout << "n_row: " << n_row << std::endl;
    std::vector<double> resultVec(n_row,0);
    
    for (int i = 0; i < n_row; i++){
        for (int k = row_ptr[i]; k < row_ptr[i+1]; k++){
            resultVec[i] += val[k] * r_n[col_idx[k]];
        } 
    }
    //std::cout << "Got outside matVecprod" << std::endl;
    return resultVec;
}

std::vector<double> scalVecProduct(const std::vector<double> &r_n, double k){
    std::vector<double> resultVec;
    for (auto r: r_n){
        resultVec.push_back(r * k);
    }
    return resultVec;
}

std::vector<double> sum2Vec(const std::vector<double> &r_m, const std::vector<double> &r_n){
    std::vector<double> resultVec;
    for (int i = 0; i < (int)r_m.size(); i++){
        resultVec.push_back(r_m[i] + r_n[i]);
    }
    return resultVec;
}

/*
int main(){
    std::vector<double> r_m = {-0.5,0,-0.5}, r_n = {1,2,3}, v;
    double l;
    l = L2norm(r_m);
    std::cout << "L2norm: " << l << std::endl;
    //std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
    return 0;
}
*/
