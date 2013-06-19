#include <iostream>
#include "rose_of_directions.hpp"

namespace yalecg {

  //####################################################//
  //##### CO-DESTRUCTOR ################################//
  //####################################################//

  rose_of_directions::rose_of_directions() {
    debug_flag_ = false;
  }

  rose_of_directions::~rose_of_directions() {
  }

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void rose_of_directions::compute_from_image_in(std::vector<float>& rose_vec, const image_t& image) {
    if (image.type() != CV_8UC1) {
      std::cerr << "ASSERT - (image.type() == CV_8UC1) failed" << std::endl;
      exit(2);
    }
    acf_t acf;
    image_t img_acf;
    acf.compute_in(img_acf,image);
    compute_from_acf_in(rose_vec,img_acf);
  }

  void rose_of_directions::compute_from_acf_in(std::vector<float>& rose_vec, const image_t& image) {
    rose_vec.clear();
    rose_vec.resize(180);
    int width  = image.size().width;
    int height = image.size().height; 
    int D0 = (width<height) ? width / 2 - 1 : height / 2 - 1;
    int d0 = D0 / 10;
    float PI = 3.14159f;
    for (int theta = 0; theta < 180; ++theta) {
      rose_vec[theta] = 0.0f;
      for (int r = d0; r < D0; ++r) {
	float x = 0.5f * float(width)  + float(r) * std::sin(PI*float(theta)/180.0f);
	float y = 0.5f * float(height) - float(r) * std::cos(PI*float(theta)/180.0f);
	rose_vec[theta] += image.at<float>(int(y),int(x));
      }
    }
    if (debug_flag_) { draw_rod(rose_vec); }
  }

  //####################################################//
  //##### PROTECTED - DEBUG ############################//
  //####################################################//

  void rose_of_directions::draw_rod(const std::vector<float>& rose_vec) const {
    std::cerr << "      --> compute_from_..._in(...)" << std::endl;
    std::srand(time(NULL));
    std::stringstream ss;
    ss << "rod_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    std::size_t rose_vec_size = rose_vec.size();
    float rod_min = 1e30f;
    float rod_max = 0.0f;
    for (std::size_t j = 0; j < rose_vec_size; ++j) {
      rod_min = std::min(rod_min,rose_vec[j]);
      rod_max = std::max(rod_max,rose_vec[j]);
    }
    int width  = int(rose_vec_size);
    int height = int(rose_vec_size*3/4);
    image_t output_image(height,width,CV_8UC3);
    output_image.setTo(cv::Vec3b(0,0,0));
    for (std::size_t j = 0; j < rose_vec_size; ++j) {
      float value = float(height-1) * (rose_vec[j] - rod_min) / (rod_max - rod_min);
      cv::Point p0, p1;
      p0.x = j;
      p1.x = j;
      p0.y = height;
      p1.y = height - int(value);
      cv::line(output_image,p0,p1,cv::Scalar(0,255,0),1);
    }
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "          - writing rod image - " << output_fname << std::endl;
  }

} // namespace yalecg
