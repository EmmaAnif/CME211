#include "image.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

int main(void){

  //declare relevant variables
  int sharp; unsigned int ret_val; std::string outfile;
  
  //load the original image
  image stanford("stanford.jpg");
  //calculate the sharpness of the original image
  sharp = stanford.Sharpness();
  std::cout << "Original Image: " << sharp << std::endl;
  
  for (int val = 3; val <= 27; val += 4){
    //reload the original image
    image stanford("stanford.jpg");
    
    //blur the original image and save the blurred image
    stanford.BoxBlur(val);
    std::stringstream outnum;
    outnum << std::setfill('0') << std::setw(2) << val;
    outfile = "BoxBlur" + outnum.str() + ".jpg";
    
    stanford.Save(outfile);
    
    //calculate the sharpness of the blurred image
    ret_val = stanford.Sharpness();
    std::cout << "BoxBlur(" << val << "): = " << ret_val << std::endl;
  }
  
  return 0;
}
