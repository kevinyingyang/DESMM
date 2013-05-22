#include <predominant_frequency_descriptor.hpp>

//####################################################//
//##### TYPEDEFS #####################################//
//####################################################//

typedef yalecg::predominant_frequency_descriptor pfd_t;
typedef cv::Mat                                  image_t;

//####################################################//
//##### MAIN #########################################//
//####################################################//

int main(int /*argc*/,char** argv) {
  std::string image_fname = argv[1];

  image_t image = cv::imread(image_fname.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
  
  std::size_t index;
  float       amplitude;

  pfd_t pfd;
  pfd.enable_debug(true);
  pfd.compute_in(index,amplitude,image);

  return 0;
}
