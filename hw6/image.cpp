#include "image.hpp"

/* read the file and store the image data as atrributes */
image::image(std::string infile) {
  ReadGrayscaleJPEG(infile, this->img);
  this->infile = infile;
}

/* writes the current version of the image to a JPEG file */
void image::Save(std::string outfile){
  if (outfile == ""){
    outfile = this->infile;
  }
  WriteGrayscaleJPEG(outfile,this->img);
}

/* smooths an image using a kernel with the given kernel size */
void image::BoxBlur(int k_size){
  int n_kernel = (int)std::pow(k_size,2);
  std::stringstream outnum; std::string outfile;
  boost::multi_array<float, 2> kernel(boost::extents[k_size][k_size]);
  
  for (int i = 0; i < k_size; i++){
    for (int j = 0; j < k_size; j++){
      kernel[i][j] = (float)1/(float)n_kernel;
    }
  }
  Convolution(this->img,this->img,kernel);
}

/* computes the sharpeness of the image */
unsigned int image::Sharpness(void){
  int k_size = 3;
  boost::multi_array<float, 2> kernel(boost::extents[k_size][k_size]);
  float kernel_arr[] = {0,1,0,1,-4,1,0,1,0}; //Laplacian operator
  float* k_ptr = &kernel_arr[0];
  
  for (int i = 0; i < k_size; i++){
    for (int j = 0; j < k_size; j++){
      kernel[i][j] = *(k_ptr++);
    }
  }
  Convolution(this->img,this->img,kernel);
  //returns max element in the multi-array
  return *std::max_element(this->img.origin(),this->img.origin()+\
      						this->img.num_elements());
}

/* computes the convolution of an image and a kernel */
void Convolution(boost::multi_array<unsigned char, 2>& input,
		boost::multi_array<unsigned char, 2>& output,
		boost::multi_array<float, 2>& kernel){
  
  int input_size = (int)input.size(), output_size = (int)output.size();
  
  //check to ensure input and output have same size
  if (input_size != output_size)
    throw std::runtime_error("Convolution(): input and output not same size");
    
  int k_row = (int)kernel.shape()[0], k_col = (int)kernel.shape()[1];

  //check to ensure that kernel is square with odd size of minimum 3
  if (k_row != k_col or k_row < 3 or k_row%2 != 1)
    throw std::runtime_error("Convolution(): kernel must be square, with kernel \
                                            size odd and greater than 3");
  
  //set up typedef for index range
  typedef boost::multi_array<double, 2>::index_range index_range;
  
  //store num of rows, cols, kernel size
  int nrows = (int)input.shape()[0], ncols = (int)input.shape()[1];
  int k_size = k_row, k_extend = (k_size-1)/2;
  
  //create enlarged multi_array extended by k_extend on all 4 sides
  int n_large_row = nrows+k_size-1, n_large_col = ncols+k_size-1;
  boost::multi_array<unsigned char,2> buffer(boost::extents[n_large_row]\
  							[n_large_col]);
 
  //--design_0
  //--Nice handling of edge cases. 
  //--START
  for (int i = 0; i < n_large_row; i++){
    for (int j = 0; j < n_large_col; j++){
      int leftmost_col = k_extend, rightmost_col = ncols + k_extend -1;
      int topmost_row = k_extend, bottomost_row = nrows + k_extend -1;
      
      if (i <= topmost_row){ //pixels on top edge
        if (j <= leftmost_col)
          buffer[i][j] = input[topmost_row-k_extend][leftmost_col-k_extend];
        else if(j >= rightmost_col)
          buffer[i][j] = input[topmost_row-k_extend][rightmost_col-k_extend];
        else
          buffer[i][j] = input[topmost_row-k_extend][j-k_extend];
      }
      
      else if (i >= bottomost_row){ //pixels on bottom edge
        if (j <= leftmost_col)
          buffer[i][j] = input[bottomost_row-k_extend][leftmost_col-k_extend];
        else if(j >= rightmost_col)
          buffer[i][j] = input[bottomost_row-k_extend][rightmost_col-k_extend];
        else
          buffer[i][j] = input[bottomost_row-k_extend][j-k_extend];
      }
      
      else if (j <= leftmost_col) //pixels on left edge
        buffer[i][j] = input[i-k_extend][leftmost_col-k_extend];
        
      else if (j >= rightmost_col) //pixels on right edge
        buffer[i][j] = input[i-k_extend][rightmost_col-k_extend];
        
      else //pixels not at the boundaries
        buffer[i][j] = input[i-k_extend][j-k_extend];
    }
  }
  //--END
  
  //convolution computation
  boost::multi_array<unsigned char,2> in_view(boost::extents[k_size][k_size]);
  for (int i = k_extend; i < n_large_row - k_extend; i++){
    for (int j = k_extend; j < n_large_col - k_extend; j++){
      in_view = buffer[boost::indices[index_range(i-k_extend,i+k_extend+1)]\
      				  [index_range(j-k_extend,j+k_extend+1)]];
      				  
      //calculate the convolution between in_view and kernel
      float sum_val = 0;
      for (int p = 0; p < k_size; p++){
        for (int q = 0; q < k_size; q++){
          sum_val += (kernel[p][q] * (float)in_view[p][q]);
        }
      }
      sum_val = std::floor(sum_val);
     
      if (sum_val > 255){
        sum_val = 255;
      }
      if (sum_val < 0){
        sum_val = 0;
      }

      output[i-k_extend][j-k_extend] = (unsigned char)sum_val;
    }
  }
}

//--functionality_0
//--Passed all tests!
//--END
