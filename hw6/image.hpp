#include <algorithm>
#include <cmath>
#include "hw6.hpp"
#include <iostream>
#include <stdexcept>

/* image class can read and write JPEG files, and has methods to compute
* the sharpness of the image and smooth (blur) the image using a box kernel
* of a specified size */
class image {

  private:
  boost::multi_array<unsigned char, 2> img; //used to store image data
  std::string infile; //name of JPEG to be read
  
  public:
    /* read the file and store the image data as an attribute */
    image(std::string infile);

    /* writes the current version of the image to a JPEG file */
    void Save(std::string outfile);
    
    /* smooths an image using a kernel with given kernel size */
    void BoxBlur(int k_size);
    
    /* computes the sharpness of the image */
    unsigned int Sharpness(void);
};

/* computes the convolution of an image and a kernel */
void Convolution(boost::multi_array<unsigned char, 2>& input,
		boost::multi_array<unsigned char, 2>& output,
		boost::multi_array<float, 2>& kernel);
