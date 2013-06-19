#include <iostream>
#include "journet_features.hpp"

namespace yalecg {

  //####################################################//
  //##### CO-DESTRUCTOR ################################//
  //####################################################//

  journet_features::journet_features() {
    image_      = 0;
    rose_vec_.clear();
    angle_gap_  = 20;
    debug_flag_ = false;
  }

  journet_features::~journet_features() {
    image_      = 0;
    rose_vec_.clear();
  }

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void journet_features::compute_in(float& angle, float& intensity, float& std,
				    float& transition, float& white_space,
				    const image_t& image) {
    if (image.type() != CV_8UC1) {
      std::cerr << "ASSERT - (image.type() == CV_8UC1) failed" << std::endl;
      exit(2);
    }
    image_ = &image;
    rose_vec_.clear();
    rod_t rod;
    rod.compute_from_image_in(rose_vec_,image);
    rod_features_in(angle,intensity,std);
    transition_in(transition);
    white_space_in(white_space);
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  void journet_features::rod_features_in(float& angle, float& intensity, float& std) const {
    std::size_t rose_vec_size = rose_vec_.size();
    angle     = -1.0f;
    intensity = -1.0f;
    std       = -1.0f;
    float rose_min = 1e30f;
    int pos_max = -1;
    for (std::size_t theta = 0; theta < rose_vec_size; ++theta) {
      float value = rose_vec_[theta];
      rose_min = std::min(rose_min,value);
      if (intensity < value) {
	intensity = value;
	pos_max   = int(theta);
	angle     = float(theta);
      }
    }
    //----- variance -----//
    float rose_max = intensity;
    float range = std::max(rose_max - rose_min,0.000001f);
    std::vector<float> rose_norm_vec;
    for (std::size_t theta = 0; theta < rose_vec_size; ++theta) {
      float value = (rose_vec_[theta] - rose_min) / range;
      rose_norm_vec.push_back(value);
    }
    float       sum     = 0.0f;
    float       sum_sqr = 0.0f;
    std::size_t count   = 0;  
    for (std::size_t theta = 0; theta < rose_vec_size; ++theta) {
      if (is_valid_theta(int(theta),pos_max)) {
	float value = rose_norm_vec[theta];
	sum     += value;
	sum_sqr += (value * value);
	count++;
      }
    }
    float fcount = float(count);
    std = (fcount * sum_sqr - sum * sum) / (fcount * (fcount - 1.0f));
  }

  bool journet_features::is_valid_theta(const int& theta, const int& pos_max) const {
    bool result = false;
    int angle_0 = pos_max-angle_gap_;
    int angle_1 = pos_max+angle_gap_;
    if ((angle_0 >= 0) && (angle_1 < 180)) {
      result = ((theta < angle_0) || (theta > angle_1));
    } else if (angle_0 < 0) {
      result = ((theta < 180+angle_0) && (theta > angle_1));
    } else {
      result = ((theta < angle_0) && (theta > angle_1-180));
    }
    return result;
  }

  void journet_features::transition_in(float& transition) const {
    int width  = image_->size().width;
    int height = image_->size().height;
    transition = 0.0f;
    for (int i = 0; i < height; ++i) {
      float diff_sum = 0.0f;
      for (int j = 0; j < width - 1; ++j) {
	float value_0 = float(image_->at<char>(i,j))   / 255.0f;
	float value_1 = float(image_->at<char>(i,j+1)) / 255.0f;
	diff_sum += std::abs(value_0 - value_1);
      }
      transition += (diff_sum / float(width));
    }
    transition /= float(height);
  }

  void journet_features::white_space_in(float& white_space) const {
    int width       = image_->size().width;
    int height      = image_->size().height;
    int half_width  = width / 2;
    int half_height = height / 2;
    white_space = 0.0f;
    for (int i = 0; i < height; ++i) {
      white_space += float(image_->at<char>(i,half_width)) / (255.0f * float(height));
    }
    for (int j = 0; j < width; ++j) {
      white_space += float(image_->at<char>(half_height,j)) / (255.0f * float(height));
    }
    white_space *= 0.5f;
  }
} // namespace yalecg
