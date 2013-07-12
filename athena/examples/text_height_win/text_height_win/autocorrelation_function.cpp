#include "stdafx.h"
#include <iostream>

#include "autocorrelation_function.hpp"

namespace yalecg {

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void autocorrelation_function::compute_in(image_t&       acf, 
					    const image_t& image,
					    const bool&    is_normalized) {
    initialize(image);

    image_t* I = new image_t(height_,width_,CV_32FC1);
    I->setTo(0.0f);
    if (image_->type() == CV_8UC1) {
      double alpha = 1.0/255.0;
      image_->convertTo(*I,CV_32FC1,alpha);
    } else if (image_->type() == CV_32FC1) {
      image_->copyTo(*I);
    } else {
      std::cerr << "WARNING - autocorrelation_function - invalid image type" << std::endl;
    }

    image_t* If = new image_t(height_,width_,CV_32FC2);
    If->setTo(cv::Vec2f(0.0f,0.0f));
    cv::dft(*I,*If,cv::DFT_COMPLEX_OUTPUT);
    
    autoconv_fft_in(If);

    cv::dft(*If,*I,cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
    
    acf.create(height_,width_,CV_32FC1);
    rearrange_image_to_acf_in(acf,I);

    if (is_normalized) { normalize_image_in(acf); }

    if (I)  { delete I;  I  = 0; }
    if (If) { delete If; If = 0; }
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  void autocorrelation_function::initialize(const image_t& image) {
    image_  = &image;
    width_  = image_->size().width;
    height_ = image_->size().height;
  }
  
  void autocorrelation_function::autoconv_fft_in(image_t* image) const {
    if (image->type() != CV_32FC2) { std::cerr << "ASSERT (type == CV_32FC2) failed" << std::endl; exit(2); }
    for (int i = 0; i < height_; ++i) {
      for (int j = 0; j < width_; ++j) {
	float real = image->at<cv::Vec2f>(i,j)[0];
	float imag = image->at<cv::Vec2f>(i,j)[1];
	image->at<cv::Vec2f>(i,j)[0] = real*real + imag*imag;
	image->at<cv::Vec2f>(i,j)[1] = 0.0f;
      }
    }
  }

  void autocorrelation_function::rearrange_image_to_acf_in(image_t& acf, const image_t* image) const {
    int H = height_/2;
    int W = width_/2;
    for (int i = 0; i <= H; ++i) {
      for (int j = 0; j <= W; ++j) {
	int acf_i;
	int acf_j;
	acf_i = H-1-i;
	acf_j = W-1-j;
	if (is_valid_idx(acf_i,acf_j)) { acf.at<float>(acf_i,acf_j) = image->at<float>(height_-1-i,width_-1-j); }
	acf_i = i+H;
	acf_j = j+W;
	if (is_valid_idx(acf_i,acf_j)) { acf.at<float>(acf_i,acf_j) = image->at<float>(i,j); }
	acf_i = H-1-i;
	acf_j = j+W;
	if (is_valid_idx(acf_i,acf_j)) { acf.at<float>(acf_i,acf_j) = image->at<float>(height_-1-i,j); }
	acf_i = i+H;
	acf_j = W-1-j;
	if (is_valid_idx(acf_i,acf_j)) { acf.at<float>(acf_i,acf_j) = image->at<float>(i,width_-1-j); }
      }
    }
  }

  bool autocorrelation_function::is_valid_idx(const int& i, const int& j) const {
    return (i >= 0) && (i < height_) && (j >= 0) && (j < width_);
  }

  void autocorrelation_function::normalize_image_in(image_t& image) const {
    float max_value = -1e30;
    float min_value = 1e30;
    for (int i = 0; i < height_; ++i) {
      for (int j = 0; j < width_; ++j) {
	float value = std::sqrt(image.at<float>(i,j)/float(height_*width_));
	max_value = std::max(max_value,value);
	min_value = std::min(min_value,value);
      }
    }
    float range = max_value - min_value;
    for (int i = 0; i < height_; ++i) {
      for (int j = 0; j < width_; ++j) {
	float value = std::sqrt(image.at<float>(i,j)/float(height_*width_));
	image.at<float>(i,j) = (value - min_value) / range;
      }
    }
  }

} // namespace yalecg
